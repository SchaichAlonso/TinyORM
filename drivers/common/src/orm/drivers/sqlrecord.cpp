#include "orm/drivers/sqlrecord.hpp"

#ifndef QT_NO_DEBUG_STREAM
#  include <QDebug>
#endif

#include "orm/drivers/constants_p.hpp"
#include "orm/drivers/exceptions/outofrangeerror.hpp"
#include "orm/drivers/utils/type_p.hpp"

TINYORM_BEGIN_COMMON_NAMESPACE

using Orm::Drivers::Constants::DOT;

namespace Orm::Drivers
{

/* public */

QStringList SqlRecord::fieldNames() const
{
    QStringList result;
    result.reserve(m_fields.size());

    for (const auto &field : m_fields)
        result << field.name();

    return result;
}

QString SqlRecord::fieldName(const size_type index) const
{
    return m_fields.value(index).name();
}

SqlRecord::size_type SqlRecord::indexOf(const QString &name) const noexcept
{
    const auto [isQualifiedName, tableName, fieldName] = getFieldNameSegments(name);

    for (size_type index = 0; index < m_fields.size(); ++index) {
        const auto &currentField = m_fields[index];
        const auto currentFieldName = currentField.name();

        /* Try to compare both, first as a unqualified name and if the qualified name was
           passed then compare using qualified name (table and field name). */
        if (currentFieldName.compare(name, Qt::CaseInsensitive) == 0 ||
            (isQualifiedName &&
             currentFieldName.compare(fieldName, Qt::CaseInsensitive) == 0 &&
             currentField.tableName().compare(tableName, Qt::CaseInsensitive) == 0)
        )
            return index;
    }

    return -1;
}

SqlField SqlRecord::field(const size_type index) const
{
    return m_fields.value(index);
}

SqlField SqlRecord::field(const QString &name) const
{
    return field(indexOf(name));
}

QVariant SqlRecord::value(const size_type index) const
{
    // Nothing to do
    if (!contains(index))
        return {};

    return m_fields[index].value();
}

QVariant SqlRecord::value(const QString &name) const
{
    return value(indexOf(name));
}

void SqlRecord::setValue(const size_type index, const QVariant &value)
{
    // Throw the OutOfRangeError exception if the record doesn't contain an index
    throwIfNotContains(index);

    m_fields[index].setValue(value);
}

void SqlRecord::setValue(const size_type index, QVariant &&value)
{
    // Throw the OutOfRangeError exception if the record doesn't contain an index
    throwIfNotContains(index);

    m_fields[index].setValue(std::move(value));
}

void SqlRecord::setValue(const QString &name, const QVariant &value)
{
    setValue(indexOf(name), value);
}

void SqlRecord::setValue(const QString &name, QVariant &&value)
{
    setValue(indexOf(name), std::move(value));
}

QVariant SqlRecord::defaultValue(const size_type index) const
{
    // Throw the OutOfRangeError exception if the record doesn't contain an index
    throwIfNotContains(index);

    return m_fields[index].defaultValue();
}

QVariant SqlRecord::defaultValue(const QString &name) const
{
    return defaultValue(indexOf(name));
}

bool SqlRecord::isNullColumn(const size_type index) const
{
    // Throw the OutOfRangeError exception if the record doesn't contain an index
    throwIfNotContains(index);

    return m_fields[index].isNullColumn();
}

bool SqlRecord::isNullColumn(const QString &name) const
{
    return isNullColumn(indexOf(name));
}

bool SqlRecord::isNull(const size_type index) const
{
    // Throw the OutOfRangeError exception if the record doesn't contain an index
    throwIfNotContains(index);

    return m_fields[index].isNull();
}

bool SqlRecord::isNull(const QString &name) const
{
    return isNull(indexOf(name));
}

void SqlRecord::setNull(const size_type index)
{
    // Throw the OutOfRangeError exception if the record doesn't contain an index
    throwIfNotContains(index);

    m_fields[index].clear();
}

void SqlRecord::setNull(const QString &name)
{
    setNull(indexOf(name));
}

void SqlRecord::append(const SqlField &field)
{
    m_fields.append(field);
}

void SqlRecord::append(SqlField &&field)
{
    m_fields.append(std::move(field));
}

void SqlRecord::insert(const size_type index, const SqlField &field)
{
   m_fields.insert(index, field);
}

void SqlRecord::insert(const size_type index, SqlField &&field)
{
    m_fields.insert(index, std::move(field));
}

void SqlRecord::replace(const size_type index, const SqlField &field)
{
    // Throw the OutOfRangeError exception if the record doesn't contain an index
    throwIfNotContains(index);

    m_fields[index] = field;
}

void SqlRecord::replace(const size_type index, SqlField &&field)
{
    // Throw the OutOfRangeError exception if the record doesn't contain an index
    throwIfNotContains(index);

    m_fields[index] = std::move(field);
}

void SqlRecord::remove(const size_type index)
{
    // Throw the OutOfRangeError exception if the record doesn't contain an index
    throwIfNotContains(index);

    m_fields.remove(index);
}

void SqlRecord::clear()
{
    m_fields.clear();
}

void SqlRecord::clearValues()
{
    for (auto &field : m_fields)
        field.clear();
}

/* private */

void SqlRecord::throwIfNotContains(const size_type index) const
{
    // Nothing to do
    if (contains(index))
        return;

    using namespace Qt::StringLiterals; // NOLINT(google-build-using-namespace)

    static const auto
    MessageTmpl = u"The field index '%1' is out of bounds, the index must be "
                   "between 0-%2 in %3()."_s;

    throw Exceptions::OutOfRangeError(
                MessageTmpl.arg(index).arg(m_fields.size() - 1).arg(__tiny_func__));
}

SqlRecord::FieldSegmentsType
SqlRecord::getFieldNameSegments(const QStringView name) noexcept
{
    // CUR drivers postgres what about more DOT-s and schema for PostgreSQL (can contain 3 dots?) silverqx
    Q_ASSERT(name.count(DOT) <= 1);

    const auto dotIndex = name.indexOf(DOT);
    const auto isQualifiedName = dotIndex > -1;

    // Nothing to do, unqualified field name
    if (!isQualifiedName)
        return {false, {}, name};

    return {.isQualifiedName = true,
            .tableName = name.first(dotIndex),
            .fieldName = name.sliced(dotIndex + 1)};
}

#ifdef TINYDRIVERS_MYSQL_DRIVER
SqlField &SqlRecord::fieldInternal(const size_type index)
{
    return m_fields[index];
}

SqlField &SqlRecord::fieldInternal(const QString &name)
{
    return fieldInternal(indexOf(name));
}
#endif

} // namespace Orm::Drivers

TINYORM_END_COMMON_NAMESPACE

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug,
                  const TINYORM_PREPEND_NAMESPACE(Orm::Drivers::SqlRecord) &record)
#endif
{
    const QDebugStateSaver saver(debug);
    debug.nospace();

    using SizeType = std::remove_cvref_t<decltype (record)>::size_type;

    const auto count = record.count();
    debug << "SqlRecord(" << count << ')';

    for (SizeType index = 0; index < count; ++index) {
        debug << '\n' << qSetFieldWidth(2) << Qt::right << index
              // Reset alignment
              << Qt::left << qSetFieldWidth(0) << ": "

              << record.field(index);
    }

    return debug;
}
