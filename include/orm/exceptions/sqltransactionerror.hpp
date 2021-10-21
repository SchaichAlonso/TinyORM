#pragma once
#ifndef ORM_SQLTRANSACTIONERROR_HPP
#define ORM_SQLTRANSACTIONERROR_HPP

#include "orm/macros/systemheader.hpp"
TINY_SYSTEM_HEADER

#include "orm/exceptions/sqlerror.hpp"

TINYORM_BEGIN_COMMON_NAMESPACE

namespace Orm::Exceptions
{

    /*! Sql transaction exception. */
    class SHAREDLIB_EXPORT SqlTransactionError : public SqlError
    {
        /*! Inherit constructors. */
        using SqlError::SqlError;
    };

} // namespace Orm

TINYORM_END_COMMON_NAMESPACE

#endif // ORM_SQLTRANSACTIONERROR_HPP
