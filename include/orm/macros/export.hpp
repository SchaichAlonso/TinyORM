#pragma once
#ifndef ORM_MACROS_EXPORT_HPP
#define ORM_MACROS_EXPORT_HPP

#include "orm/macros/systemheader.hpp"
TINY_SYSTEM_HEADER

#include "orm/macros/export_common.hpp"

#ifdef TINYORM_BUILDING_SHARED
#  define SHAREDLIB_EXPORT TINY_DECL_EXPORT
#elif defined(TINYORM_LINKING_SHARED)
#  define SHAREDLIB_EXPORT TINY_DECL_IMPORT
#endif

// Building library archive (static)
#ifndef SHAREDLIB_EXPORT
#  define SHAREDLIB_EXPORT
#endif

#endif // ORM_MACROS_EXPORT_HPP
