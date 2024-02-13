#pragma once
#ifndef ORM_EXCEPTIONS_SQLERROR_HPP
#define ORM_EXCEPTIONS_SQLERROR_HPP

#include "orm/macros/systemheader.hpp"
TINY_SYSTEM_HEADER

#include <QtSql/QSqlError>

#include "orm/exceptions/runtimeerror.hpp"

TINYORM_BEGIN_COMMON_NAMESPACE

namespace Orm::Exceptions
{

    /*! TinyORM SqlError exception, wrapper for the QSqlError. */
    class SHAREDLIB_EXPORT SqlError : public RuntimeError // clazy:exclude=copyable-polymorphic
    {
    public:
        /*! const char * constructor. */
        SqlError(const char *message, const QSqlError &error);
        /*! QString constructor. */
        SqlError(const QString &message, const QSqlError &error);

        /*! const char * constructor. */
        SqlError(const char *message, QSqlError &&error);
        /*! QString constructor. */
        SqlError(const QString &message, QSqlError &&error);

        /*! Get the original Qt SQL error. */
        inline const QSqlError &getSqlError() const noexcept;

    protected:
        /*! Protected converting constructor for use by descendants to avoid an error
            message formatting. */
        SqlError(const QString &message, const QSqlError &error, int /*unused*/);

        /*! Format the Qt SQL error message. */
        static QString formatMessage(const char *message, const QSqlError &error);

        /*! The Qt SQL error instance. */
        QSqlError m_sqlError;
    };

    /* public */

    const QSqlError &SqlError::getSqlError() const noexcept
    {
        return m_sqlError;
    }

} // namespace Orm::Exceptions

TINYORM_END_COMMON_NAMESPACE

#endif // ORM_EXCEPTIONS_SQLERROR_HPP
