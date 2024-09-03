#pragma once
#ifndef ORM_DRIVERS_CONSTANTS_EXTERN_P_HPP
#define ORM_DRIVERS_CONSTANTS_EXTERN_P_HPP

#include <QString>

#include <orm/macros/commonnamespace.hpp>

TINYORM_BEGIN_COMMON_NAMESPACE

namespace Orm::Drivers
{
    /*! Alias for the literal operator that creates a QString. */
    using Qt::StringLiterals::operator""_s;

/*! Namespace contains common chars and strings used in the TinyDrivers project. */
namespace Constants
{

    // Common chars
    extern const QChar SPACE;
    extern const QChar DOT;
    extern const QChar QUOTE;

    // Common strings
    extern const QString NEWLINE;
    extern const QString COMMA;
    extern const QString null_;

    // Database related
    extern const QString QMYSQL;
//    extern const QString QPSQL;
//    extern const QString QSQLITE;

    // Others
    extern const QString NotImplemented;

} // namespace Constants
} // namespace Orm::Drivers

TINYORM_END_COMMON_NAMESPACE

#endif // ORM_DRIVERS_CONSTANTS_EXTERN_P_HPP
