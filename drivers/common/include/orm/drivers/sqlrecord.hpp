#pragma once
#ifndef ORM_DRIVERS_SQLRECORD_HPP
#define ORM_DRIVERS_SQLRECORD_HPP

#include <orm/macros/systemheader.hpp>
TINY_SYSTEM_HEADER

#include "orm/drivers/sqlfield.hpp"

TINYORM_BEGIN_COMMON_NAMESPACE

namespace Orm::Drivers
{

    /*! The SqlRecord represents a database row. */
    class TINYDRIVERS_EXPORT SqlRecord
    {
    public:
        /* Container related */
        /*! Alias for the SqlRecord size type. */
        using size_type = int;

        /*! Default constructor. */
        SqlRecord() = default;
        /*! Default destructor. */
        ~SqlRecord() = default;

        /*! Copy constructor. */
        SqlRecord(const SqlRecord &) = default;
        /*! Copy assignment operator. */
        SqlRecord &operator=(const SqlRecord &) = default;

        /*! Move constructor. */
        SqlRecord(SqlRecord &&) noexcept = default;
        /*! Move assignment operator. */
        SqlRecord &operator=(SqlRecord &&) noexcept = default;

        /*! Equality comparison operator for the SqlRecord. */
        bool operator==(const SqlRecord &) const = default;

        /*! Swap the SqlRecord. */
        inline void swap(SqlRecord &other) noexcept;

        /*! Get the field name at the given index. */
        QString fieldName(size_type index) const;
        /*! Get the index of the given field name (-1 if it can't be found). */
        size_type indexOf(const QString &name) const noexcept;

        /*! Get the field at the given index. */
        SqlField field(size_type index) const;
        /*! Get the field by field name. */
        SqlField field(const QString &name) const;

        /*! Get the field value at the given index. */
        QVariant value(size_type index) const;
        /*! Get the field value by field name. */
        QVariant value(const QString &name) const;

        /*! Set the value of the field at the given index to the given value. */
        void setValue(size_type index, const QVariant &value);
        /*! Set the value of the field at the given index to the given value. */
        void setValue(size_type index, QVariant &&value);

        /*! Set the value of the field with the field name to the given value. */
        void setValue(const QString &name, const QVariant &value);
        /*! Set the value of the field with the field name to the given value. */
        void setValue(const QString &name, QVariant &&value);

        /*! Determine whether a field is SQL nullable (NULL in the table definition). */
        bool isNullColumn(size_type index) const;
        /*! Determine whether a field is SQL nullable (NULL in the table definition). */
        bool isNullColumn(const QString &name) const;

        /*! Determine whether the field at the given index is NULL. */
        bool isNull(size_type index) const;
        /*! Determine whether the field with the given field name is NULL. */
        bool isNull(const QString &name) const;

        /*! Set the value of the field at the given index to NULL. */
        void setNull(size_type index);
        /*! Set the value of the field with the given field name to NULL. */
        void setNull(const QString &name);

        /*! Append a copy of the given field to the end of the record. */
        void append(const SqlField &field);
        /*! Append the given field to the end of the record. */
        void append(SqlField &&field);

        /*! Insert a copy of the given field at the index position in the record. */
        void insert(size_type index, const SqlField &field);
        /*! Insert the given field at the index position in the record. */
        void insert(size_type index, SqlField &&field);

        /*! Replace the field at the index position with the given field. */
        void replace(size_type index, const SqlField &field);
        /*! Replace the field at the index position with the given field. */
        void replace(size_type index, SqlField &&field);

        /*! Remove the field at the given index position from the record. */
        void remove(size_type index);

        /*! Removes all field from the record. */
        void clear();
        /*! Clear all field values in the record and set each field to NULL. */
        void clearValues();

        /*! Determine whether the current record contains fields. */
        inline bool isEmpty() const noexcept;
        /*! Get the number of fields in the current record. */
        inline size_type count() const noexcept;
        /*! Allocate memory for at least the given size of fields. */
        inline void reserve(size_type size);

        /*! Determine whether the current record contains the given field name. */
        inline bool contains(const QString &name) const noexcept;
        /*! Determine whether the current record contains the given field index. */
        inline bool contains(size_type index) const noexcept;

    private:
        /*! Throw the OutOfRangeError exception if record doesn't contain an index. */
        void throwIfNotContains(size_type index) const;

        /*! FieldSegmentsType return type for getFieldNameSegments(). */
        struct FieldSegmentsType
        {
            /*! Determine whether a given field is qualified. */
            bool isQualifiedName = false;
            /*! Table name. */
            QStringView tableName;
            /*! Field name. */
            QStringView fieldName;
        };

        /*! Get individual segments from the aliased field identifier
            (column alias (select expression)). */
        static FieldSegmentsType getFieldNameSegments(QStringView name) noexcept;

        /* Data members */
        /*! Record fields. */
        QList<SqlField> m_fields;
    };

    /* public */

    void SqlRecord::swap(SqlRecord &other) noexcept
    {
        m_fields.swap(other.m_fields);
    }

    bool SqlRecord::isEmpty() const noexcept
    {
        return m_fields.isEmpty();
    }

    SqlRecord::size_type SqlRecord::count() const noexcept
    {
        return static_cast<size_type>(m_fields.size());
    }

    void SqlRecord::reserve(const size_type size)
    {
        m_fields.reserve(size);
    }

    bool SqlRecord::contains(const QString &name) const noexcept
    {
        return indexOf(name) >= 0;
    }

    bool SqlRecord::contains(const size_type index) const noexcept
    {
      return index >= 0 && index < m_fields.size();
    }

} // namespace Orm::Drivers

TINYORM_END_COMMON_NAMESPACE

#ifndef QT_NO_DEBUG_STREAM
TINYDRIVERS_EXPORT QDebug
operator<<(QDebug debug, const TINYORM_PREPEND_NAMESPACE(Orm::Drivers::SqlRecord) &record);
#endif

#endif // ORM_DRIVERS_SQLRECORD_HPP
