#pragma once
#ifndef ORM_DRIVERS_DRIVERSTYPES_HPP
#define ORM_DRIVERS_DRIVERSTYPES_HPP

#include <orm/macros/systemheader.hpp>
TINY_SYSTEM_HEADER

#include <QtTypes>

#include <orm/macros/commonnamespace.hpp>

TINYORM_BEGIN_COMMON_NAMESPACE

namespace Orm::Drivers
{

    /*! Result set cursor position type. */
    enum CursorPosition : qint8
    {
        /*! Cursor position before the first row. */
        BeforeFirstRow = -1,
        /*! Cursor position after the last row. */
        AfterLastRow = -2
    };

    /*! Numerical precision policies for floating point numbers. */
    enum struct NumericalPrecisionPolicy : qint8
    {
        /*! Force 32bit integer values. The fractional part is silently discarded. */
        LowPrecisionInt32    = 0x01,
        /*! Force 64bit integer values. The fractional part is silently discarded. */
        LowPrecisionInt64    = 0x02,
        /*! Force the double values. This is the default policy. */
        LowPrecisionDouble   = 0x04,
        /*! QString will be used to preserve precision. */
        HighPrecision        = 0
    };

    /*! Type of a bind parameter (dummy type for API compatibility). */
    enum struct ParamType : qint8
    {
        /*! The bind parameter to put data into the database. */
        In = 0,
    };

} // namespace Orm::Drivers

TINYORM_END_COMMON_NAMESPACE

#endif // ORM_DRIVERS_DRIVERSTYPES_HPP
