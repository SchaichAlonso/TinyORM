#pragma once
#ifndef TINYUTILS_VERSION_HPP
#define TINYUTILS_VERSION_HPP

#define TINYUTILS_VERSION_MAJOR 0
#define TINYUTILS_VERSION_MINOR 1
#define TINYUTILS_VERSION_BUGFIX 0
#define TINYUTILS_VERSION_BUILD 0

#define TINYUTILS__STRINGIFY(x) #x
#define TINYUTILS_STRINGIFY(x) TINYUTILS__STRINGIFY(x)

#if TINYUTILS_VERSION_BUILD != 0
#  define TINYUTILS_PROJECT_VERSION TINYUTILS_STRINGIFY( \
     TINYUTILS_VERSION_MAJOR.TINYUTILS_VERSION_MINOR.TINYUTILS_VERSION_BUGFIX.TINYUTILS_VERSION_BUILD \
   )
#else
#  define TINYUTILS_PROJECT_VERSION TINYUTILS_STRINGIFY( \
     TINYUTILS_VERSION_MAJOR.TINYUTILS_VERSION_MINOR.TINYUTILS_VERSION_BUGFIX \
   )
#endif

/* Version Legend:
   M = Major, m = minor, p = patch, t = tweak, s = status ; [] - excluded if 0 */

// Format - M.m.p.t (used in Windows RC file)
#define TINYUTILS_FILEVERSION_STR TINYUTILS_STRINGIFY( \
    TINYUTILS_VERSION_MAJOR.TINYUTILS_VERSION_MINOR.TINYUTILS_VERSION_BUGFIX.TINYUTILS_VERSION_BUILD)
// Format - M.m.p[.t]-s
#define TINYUTILS_VERSION_STR TINYUTILS_PROJECT_VERSION
// Format - vM.m.p[.t]-s
#define TINYUTILS_VERSION_STR_2 "v" TINYUTILS_PROJECT_VERSION

/*! Version number macro, can be used to check API compatibility, format - MMmmpp. */
#define TINYUTILS_VERSION \
    (TINYUTILS_VERSION_MAJOR * 10000 + TINYUTILS_VERSION_MINOR * 100 + TINYUTILS_VERSION_BUGFIX)

#endif // TINYUTILS_VERSION_HPP
