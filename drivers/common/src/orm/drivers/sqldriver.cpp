#include "orm/drivers/sqldriver.hpp"

#include "orm/drivers/constants_p.hpp"
#include "orm/drivers/dummysqlerror.hpp"
#include "orm/drivers/sqldriver_p.hpp"

TINYORM_BEGIN_COMMON_NAMESPACE

using Orm::Drivers::Constants::QUOTE;

namespace Orm::Drivers
{

/* protected */

SqlDriver::SqlDriver(std::unique_ptr<SqlDriverPrivate> &&dd) noexcept
    : d_ptr(std::move(dd))
{}

/* public */

/* The destructor must be in the cpp file because the d_ptr is unique_ptr.
   If the destructor is inline then the compilation fails because a unique_ptr can't
   destroy an incomplete type. */
SqlDriver::~SqlDriver() = default;

/* Getters / Setters */

bool SqlDriver::isOpen() const noexcept
{
    Q_D(const SqlDriver);
    return d->isOpen;
}

bool SqlDriver::isOpenError() const noexcept // NOLINT(readability-convert-member-functions-to-static)
{
    // Must be defined to be API compatible with the QtSql module

    return false;
}

QString SqlDriver::connectionName() const noexcept
{
    Q_D(const SqlDriver);
    return d->connectionName;
}

SqlDriver::DbmsType SqlDriver::dbmsType() const noexcept
{
    Q_D(const SqlDriver);
    return d->dbmsType;
}

DummySqlError SqlDriver::lastError() const noexcept // NOLINT(readability-convert-member-functions-to-static)
{
    // Must be defined to be API compatible with the QtSql module

    return {};
}

NumericalPrecisionPolicy SqlDriver::defaultNumericalPrecisionPolicy() const noexcept
{
    Q_D(const SqlDriver);
    return d->defaultPrecisionPolicy;
}

void
SqlDriver::setDefaultNumericalPrecisionPolicy(
        const NumericalPrecisionPolicy precision) noexcept
{
    Q_D(SqlDriver);
    d->defaultPrecisionPolicy = precision;
}

std::thread::id SqlDriver::threadId() const noexcept
{
    Q_D(const SqlDriver);
    return d->threadId;
}

/* Others */

int SqlDriver::maximumIdentifierLength(
        const SqlDriver::IdentifierType /*unused*/) const noexcept
{
    return std::numeric_limits<int>::max();
}

QString
SqlDriver::stripDelimiters(const QString &identifier, const IdentifierType type) const
{
    // Nothing to do
    if (!isIdentifierEscaped(identifier, type))
        return identifier;

    return identifier.sliced(1).chopped(1);
}

/* The following two methods are named wrong, but I can't rename them because
   they are public and virtual. 🤔 */
bool SqlDriver::isIdentifierEscaped(const QString &identifier,
                                    const IdentifierType /*unused*/) const
{
    return identifier.size() > 2 &&
           identifier.startsWith(QUOTE) &&
           identifier.endsWith(QUOTE);
}

/* protected */

/* Setters */

void SqlDriver::setOpen(const bool value) noexcept
{
    Q_D(SqlDriver);
    d->isOpen = value;
}

void SqlDriver::setConnectionName(const QString &connection) noexcept
{
    Q_D(SqlDriver);
    d->connectionName = connection;
}

} // namespace Orm::Drivers

TINYORM_END_COMMON_NAMESPACE
