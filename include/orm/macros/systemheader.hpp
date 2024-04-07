#pragma once
#ifndef ORM_MACROS_SYSTEMHEADER_HPP
#define ORM_MACROS_SYSTEMHEADER_HPP

/* The system_header pragma is disabled if building the TinyORM library itself
   as the shared library. */
#if !defined(TINYORM_PRAGMA_SYSTEM_HEADER_OFF) &&                                       \
    (!defined(TINYORM_BUILDING_SHARED) || !defined(TINYUTILS_BUILDING_SHARED) ||        \
     !defined(TINYDRIVERS_BUILDING_SHARED))
// Clang masquerades as GCC 4.2.0 so it has to be first
#  ifdef __clang__
#    define TINY_SYSTEM_HEADER _Pragma("clang system_header")
#  elif __GNUC__ * 100 + __GNUC_MINOR__ > 301
#    define TINY_SYSTEM_HEADER _Pragma("GCC system_header")
#  elif defined(_MSC_VER)
#    define TINY_SYSTEM_HEADER _Pragma("system_header")
#  endif
#endif

#ifndef TINY_SYSTEM_HEADER
#  define TINY_SYSTEM_HEADER
#endif

TINY_SYSTEM_HEADER

#endif // ORM_MACROS_SYSTEMHEADER_HPP
