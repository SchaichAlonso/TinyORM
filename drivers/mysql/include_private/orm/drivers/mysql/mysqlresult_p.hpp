#pragma once
#ifndef ORM_DRIVERS_MYSQL_MYSQLRESULT_P_HPP
#define ORM_DRIVERS_MYSQL_MYSQLRESULT_P_HPP

#include "orm/drivers/macros/declaresqldriverprivate_p.hpp"
#include "orm/drivers/mysql/mysqldriver.hpp"
#include "orm/drivers/mysql/mysqltypes_p.hpp"
#include "orm/drivers/sqlresult_p.hpp"
#include "orm/drivers/support/sqlrecordcache_p.hpp"

/* All the in vs out bindings/data-related comments and identifier names related
   to the results or prepared bindings are described from the MySQL server perspective and
   they follow the MySQL documentation conventions.
   Input bindings are prepared statements and output bindings are results from the MySQL
   server. Is a very bad idea to switch these naming conventions. 🤔
   Instead of using the input/in and output/out words I'm using prepared and result words
   where possible to avoid confusion. */

TINYORM_BEGIN_COMMON_NAMESPACE

namespace Orm::Drivers::MySql
{

    /*! MySqlResult private implementation. */
    class MySqlResultPrivate : public SqlResultPrivate
    {
        Q_DISABLE_COPY_MOVE(MySqlResultPrivate)

    public:
        TINY_DECLARE_SQLDRIVER_PRIVATE(MySqlDriver)

        /*! Inherit constructors. */
        using SqlResultPrivate::SqlResultPrivate;

        /*! Default destructor. */
        ~MySqlResultPrivate() = default;

        /*! MySQL field type (used for result sets' fields). */
        struct MyField
        {
            /* Common for both */
            /*! Field metatype. */
            QMetaType metaType;
            /*! Field metadata. */
            const MYSQL_FIELD *myField = nullptr; // Returned by mysql_fetch_field()

            /* Prepared queries */
            /*! Field value buffer in the result set. */
            std::unique_ptr<char[]> fieldValue = nullptr; // NOLINT(modernize-avoid-c-arrays)
            /*! Is the field NULL? */
            my_bool isNull = false;
            /*! Field value buffer length w/o terminating null character. */
            ulong fieldValueSize = 0UL; // For strings, it varies by character set (latin1 1 byte or eg. utf8mb4 4 bytes so 3 characters string will have size 12)
        };

        /* Normal queries */
        /*! Populate the result fields vector. */
        bool populateFields(MYSQL *mysql);

        /* Prepared queries */
        /*! Bind result set values into the resultBinds data member. */
        bool bindResultValues();

        /*! Determine whether the correct number of prepared bindings was bound. */
        bool shouldPrepareBindings(uint placeholdersCount) const;
        /*! Check the correct prepared bindings count and show warnings. */
        static void checkPreparedBindingsCount(uint placeholdersCount, ulong valuesSize);

        /*! Bind prepared bindings for placeholders into the preparedBinds data member. */
        void bindPreparedBindings(
                QList<my_bool> &nullVector, QList<QByteArray> &stringVector,
                QList<MYSQL_TIME> &timeVector);

        /*! Bind result set BLOB values. */
        void bindResultBlobs();

        /* Result sets */
        /*! Alias for the result fields vector type. */
        using ResultFieldsType = std::vector<MyField>;
        /*! Alias for the result fields vector size type. */
        using ResultFieldsSizeType = ResultFieldsType::size_type;

        /*! Get the error message based on the error code from the mysql_stmt_fetch(). */
        static std::optional<QString> errorMessageForStmtFetch(int status) noexcept;

        /*! Obtain the QVariant value for normal queries. */
        QVariant getValueForNormal(ResultFieldsSizeType index) const;
        /*! Obtain the QVariant value for prepared queries. */
        QVariant getValueForPrepared(ResultFieldsSizeType index) const;

        /*! Throw an exception if an index for result fields vector is out of bounds. */
        void throwIfBadResultFieldsIndex(size_type index) const;

        /* Data members */
        /* Common for both */
        /*! Fields for the currently obtained record/row. */
        ResultFieldsType resultFields;

        /* Normal queries */
        /*! Result set handle (from the mysql_store_result()). */
        MYSQL_RES *result = nullptr;
        /*! Pointer to the row in the result set (from the mysql_fetch_row()). */
        MYSQL_ROW row = nullptr;

        /* Prepared queries */
        /*! Prepared statement handler. */
        MYSQL_STMT *stmt = nullptr;
        /*! Result set metadata for a prepared statement. */
        MYSQL_RES *meta = nullptr;

        /*! Array for bind buffers to result set columns (result values returned from
            the database server). */
        std::unique_ptr<MYSQL_BIND[]> resultBinds = nullptr; // NOLINT(modernize-avoid-c-arrays)
        /*! Array for prepared bindings (data values sent to the server). */
        std::unique_ptr<MYSQL_BIND[]> preparedBinds = nullptr; // NOLINT(modernize-avoid-c-arrays)

        /* Common for both */
        /*! Has the current result set any BLOB type field/s? */
        bool hasBlobs = false;
        /*! Is the current result set for the prepared statement? */
        bool preparedQuery = false;

        /*! Cache for the record/WithDefaultValuesCached() method. */
        mutable Support::SqlRecordCachePrivate recordCache;

    private:
        /* Prepared queries */
        /*! Verify whether all fields metadata were fetched. */
        static bool wasAllFieldsFetched(uint fieldsCount, uint lastIndex,
                                        QLatin1StringView method);

        /*! Allocate memory for prepared bindings that will be sent to the database. */
        static void allocateMemoryForBindings(std::unique_ptr<MYSQL_BIND[]> &binds, // NOLINT(modernize-avoid-c-arrays)
                                              std::size_t count) noexcept;
        /*! Reserve all vectors for prepared bindings buffer data. */
        void reserveVectorsForBindings(
                QList<my_bool> &nullVector, QList<QByteArray> &stringVector,
                QList<MYSQL_TIME> &timeVector) const;

        /*! Determine whether the given MySQL field type is a BLOB. */
        static bool isBlobType(enum_field_types fieldType) noexcept;
        /*! Convert Qt date/time type to the MYSQL_TIME. */
        static MYSQL_TIME toMySqlDateTime(const QVariant &boundValue, MYSQL_BIND &bind);
        /*! Create and zero the MYSQL_TIME structure as 0000-00-00 00:00:00.000000. */
        static MYSQL_TIME createMsqlTime() noexcept;

        /* Result sets */
        /*! Determine whether the given MySQL field type is a Bit-value type. */
        inline static bool isBitType(enum_field_types type) noexcept;
        /*! Convert the Bit-value field to the quint64. */
        static quint64 toBitField(const MyField &field, const char *fieldValue) noexcept;

        /*! Convert the DATE value to the QDate. */
        static QVariant toQDateFromString(const QString &value);
        /*! Convert the TIME value to the QTime. */
        // static QVariant toQTimeFromString(const QString &value);
        /*! Convert the DATETIME value to the QDateTime. */
        static QVariant toQDateTimeFromString(QString value);
        /*! Convert the DATE/TIME value to the QDateTime (prepared statements only). */
        static QVariant toQDateTimeFromMySQLTime(int typeId,
                                                 const MYSQL_TIME *mysqlTime);

        /*! Convert the Fixed/Floating-Point value type based on the set numerical
            precision policy. */
        QVariant toDoubleFromString(const QString &value) const;
        /*! Convert the BLOB value type to the QByteArray. */
        QVariant toQByteArray(ResultFieldsSizeType index) const;

        /*! Create a QVariant by the given metatype ID and field value. */
        QVariant createQVariant(int typeId, QString &&value) const;
        /*! Create an integer QVariant by the given metatype ID and value. */
        static QVariant createIntegerQVariant(int typeId, const MyField &field);
    };

    /* private */

    /* Result sets */

    bool MySqlResultPrivate::isBitType(const enum_field_types type) noexcept
    {
        return type == MYSQL_TYPE_BIT;
    }

} // namespace Orm::Drivers::MySql

TINYORM_END_COMMON_NAMESPACE

#endif // ORM_DRIVERS_MYSQL_MYSQLRESULT_P_HPP
