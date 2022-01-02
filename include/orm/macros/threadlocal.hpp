#pragma once
#ifndef ORM_MACROS_THREADLOCAL_HPP
#define ORM_MACROS_THREADLOCAL_HPP

#include "orm/macros/systemheader.hpp"
TINY_SYSTEM_HEADER

#include <QtConfig>

// BUG TLS, clang linux crashes, gcc mingw lld/ld doesn't compile (duplicit tls), clang mingw ld stdlib weird 100,000 linker errors (COMDAT symbol xyz does not match section name), clang mingw lld stdlib works, but I'm giving up this clang MinGW support because still occasional crashes on TLS exit/destruction, it crashes around 1 time from 5, and now I have discovered that Clang 13 on Linux works, something was fixed in Clang 13?, so I'm enabling support for Clang >=13 on Linux, AND NOW with clang >= 13 and Qt 6.2.2 crashes again, Qt 5.15.2 is not crashing, Clang is king 😂🤣🤯, 02.01.2022 - now I have discovered that TinyOrmPlayground crashes even with Clang 13 and Qt 5.15.2, so I'm disabling thread_local for Clang on Linux silverqx
#if !defined(__clang__) &&                                                 \
    !(defined(__GNUG__) && !defined(__clang__) && defined(__MINGW32__)) && \
    !defined TINYORM_DISABLE_THREAD_LOCAL
#  define T_THREAD_LOCAL thread_local
#endif

#if !defined(T_THREAD_LOCAL)
#  define T_THREAD_LOCAL
#endif

#endif // ORM_MACROS_THREADLOCAL_HPP
