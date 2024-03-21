#!/usr/bin/env python
#
#===- run-clazy-standalone.py - Parallel clazy-standalone runner --------*- python -*--===#
#
# Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
#===-----------------------------------------------------------------------===#


r'''
Parallel clazy-standalone runner
==========================

Runs clazy-standalone over all files in a compilation database.
Requires clazy-standalone on $PATH.

Example invocations.
- Run clazy-standalone on all files in the current working directory with a default
  set of checks and show warnings in the cpp files and all project headers.
    run-clazy-standalone.py $PWD

- Run clazy-standalone with all level0 checks except qenums check.
    run-clazy-standalone.py -checks='level0,no-qenums'

- Show diagnostics also from header files in include/orm/ folder.
    run-clazy-standalone.py -checks='level0,level1' \
                            -header-filter='/orm/.+\.(h|hpp)$' '.+\.cpp$'

Compilation database setup:
http://clang.llvm.org/docs/HowToSetupToolingForLLVM.html
'''

from __future__ import print_function

import argparse
import glob
import json
import multiprocessing
import os
import re
import shutil
import subprocess
import sys
import tempfile
import threading
import traceback

try:
  import yaml
except ImportError:
  yaml = None

is_py2 = sys.version[0] == '2'

if is_py2:
    import Queue as queue
else:
    import queue as queue


def find_compilation_database(path):
  """Adjusts the directory until a compilation database is found."""
  result = './'
  while not os.path.isfile(os.path.join(result, path)):
    if os.path.realpath(result) == '/':
      print('Error: could not find compilation database.')
      sys.exit(1)
    result += '../'
  return os.path.realpath(result)


def make_absolute(f, directory):
  if os.path.isabs(f):
    return f
  return os.path.normpath(os.path.join(directory, f))


def get_clazy_invocation(f, clazy_binary, checks, tmpdir, build_path,
                         header_filter, extra_arg, extra_arg_before,
                         ignore_dirs):
  """Gets a command line for clazy-standalone."""
  start = [clazy_binary]
  if header_filter is not None:
    start.append('-header-filter=' + header_filter)
  if ignore_dirs is not None:
    start.append('-ignore-dirs=' + ignore_dirs)
  if checks:
    start.append('-checks=' + checks)
  if tmpdir is not None:
    start.append('-export-fixes')
    # Get a temporary file. We immediately close the handle so clazy-standalone can
    # overwrite it.
    (handle, name) = tempfile.mkstemp(suffix='.yaml', dir=tmpdir)
    os.close(handle)
    start.append(name)
  for arg in extra_arg:
      start.append('-extra-arg=%s' % arg)
  for arg in extra_arg_before:
      start.append('-extra-arg-before=%s' % arg)
  start.append('-p=' + build_path)
  start.append(f)
  return start


def merge_replacement_files(tmpdir, mergefile):
  """Merge all replacement files in a directory into a single file"""
  # The fixes suggested by clang-tidy >= 4.0.0 are given under
  # the top level key 'Diagnostics' in the output yaml files
  mergekey = "Diagnostics"
  merged=[]
  for replacefile in glob.iglob(os.path.join(tmpdir, '*.yaml')):
    content = yaml.safe_load(open(replacefile, 'r'))
    if not content:
      continue # Skip empty files.
    merged.extend(content.get(mergekey, []))

  if merged:
    # MainSourceFile: The key is required by the definition inside
    # include/clang/Tooling/ReplacementsYaml.h, but the value
    # is actually never used inside clang-apply-replacements,
    # so we set it to '' here.
    output = {'MainSourceFile': '', mergekey: merged}
    with open(mergefile, 'w') as out:
      yaml.safe_dump(output, out)
  else:
    # Empty the file:
    open(mergefile, 'w').close()


def run_clazy(args, tmpdir, build_path, queue, lock, failed_files):
  """Takes filenames out of queue and runs clazy-standalone on them."""
  while True:
    name = queue.get()
    invocation = get_clazy_invocation(name, args.clazy_binary, args.checks,
                                      tmpdir, build_path, args.header_filter,
                                      args.extra_arg, args.extra_arg_before,
                                      args.ignore_dirs)

    proc = subprocess.Popen(invocation, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, err = proc.communicate()
    if proc.returncode != 0:
      if proc.returncode < 0:
        msg = "%s: terminated by signal %d\n" % (name, -proc.returncode)
        err += msg.encode('utf-8')
      failed_files.append(name)
    with lock:
      sys.stdout.write(' '.join(invocation) + '\n' + output.decode('utf-8'))
      if len(err) > 0:
        sys.stdout.flush()
        sys.stderr.write(err.decode('utf-8'))
    queue.task_done()


def main():
  parser = argparse.ArgumentParser(description='Runs clazy-standalone over all files '
                                   'in a compilation database. Requires '
                                   'clazy-standalone on $PATH.')
  parser.add_argument('-clazy-binary', metavar='PATH',
                      default='clazy-standalone',
                      help='path to clazy-standalone binary')
  parser.add_argument('-checks', default=None,
                      help='checks filter, when not specified, use clazy-standalone '
                      'default')
  parser.add_argument('-header-filter', default=None,
                      help='regular expression matching the names of the '
                      'headers to output diagnostics from. Diagnostics from '
                      'the main file of each translation unit are always '
                      'displayed.')
  parser.add_argument('-ignore-dirs', default=None,
                      help='Regular expression matching the names of the directories '
                      'for which diagnostics should never be emitted. Useful for '
                      'ignoring 3rdparty code.')
  if yaml:
    parser.add_argument('-export-fixes', metavar='filename', dest='export_fixes',
                        help='Create a yaml file to store suggested fixes in, '
                        'which can be applied with clang-apply-replacements.')
  parser.add_argument('-j', type=int, default=0,
                      help='number of clazy-standalone instances to be run in parallel.')
  parser.add_argument('files', nargs='*', default=['.*'],
                      help='files to be processed (regex on path)')
  parser.add_argument('-p', dest='build_path',
                      help='Path used to read a compile command database.')
  parser.add_argument('-extra-arg', dest='extra_arg',
                      action='append', default=[],
                      help='Additional argument to append to the compiler '
                      'command line.')
  parser.add_argument('-extra-arg-before', dest='extra_arg_before',
                      action='append', default=[],
                      help='Additional argument to prepend to the compiler '
                      'command line.')
  args = parser.parse_args()

  db_path = 'compile_commands.json'

  if args.build_path is not None:
    build_path = args.build_path
  else:
    # Find our database
    build_path = find_compilation_database(db_path)

  try:
    invocation = [args.clazy_binary, '-list-checks']
    invocation.append('-p=' + build_path)
    if args.checks:
      invocation.append('-checks=' + args.checks)
    invocation.append('-')
    subprocess.check_call(invocation)
  except:
    print(invocation)
    print("Unable to run clazy-standalone.", file=sys.stderr)
    sys.exit(1)

  # Load the database and extract all files.
  database = json.load(open(os.path.join(build_path, db_path)))
  files = set([make_absolute(entry['file'], entry['directory'])
           for entry in database])

  max_task = args.j
  if max_task == 0:
    max_task = multiprocessing.cpu_count()

  tmpdir = None
  if yaml and args.export_fixes:
    tmpdir = tempfile.mkdtemp()

  # Build up a big regex filter from all command line arguments.
  file_name_re = re.compile('|'.join(args.files))

  return_code = 0
  try:
    # Spin up a bunch of clazy-standalone launching threads.
    task_queue = queue.Queue(max_task)
    # List of files with a non-zero return code.
    failed_files = []
    lock = threading.Lock()
    for _ in range(max_task):
      t = threading.Thread(target=run_clazy,
                           args=(args, tmpdir, build_path, task_queue, lock, failed_files))
      t.daemon = True
      t.start()

    # Fill the queue with files.
    for name in files:
      if file_name_re.search(name):
        task_queue.put(name)

    # Wait for all threads to be done.
    task_queue.join()
    if len(failed_files):
      return_code = 1

  except KeyboardInterrupt:
    # This is a sad hack. Unfortunately subprocess goes
    # bonkers with ctrl-c and we start forking merrily.
    print('\nCtrl-C detected, goodbye.')
    if tmpdir:
      shutil.rmtree(tmpdir)
    os.kill(0, 9)

  if yaml and args.export_fixes:
    print('Writing fixes to ' + args.export_fixes + ' ...')
    try:
      merge_replacement_files(tmpdir, args.export_fixes)
    except:
      print('Error exporting fixes.\n', file=sys.stderr)
      traceback.print_exc()
      return_code=1

  if tmpdir:
    shutil.rmtree(tmpdir)
  sys.exit(return_code)


if __name__ == '__main__':
  main()
