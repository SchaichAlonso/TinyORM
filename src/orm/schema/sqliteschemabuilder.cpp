#include "orm/schema/sqliteschemabuilder.hpp"

#include <filesystem>
#include <fstream>

#include "orm/databaseconnection.hpp"
#include "orm/schema/grammars/sqliteschemagrammar.hpp"
#include "orm/utils/type.hpp"

namespace fs = std::filesystem;

using fspath = std::filesystem::path;

TINYORM_BEGIN_COMMON_NAMESPACE

namespace Orm::SchemaNs
{

/* public */

std::optional<QSqlQuery> SQLiteSchemaBuilder::createDatabase(const QString &name) const
{
    fspath databaseName(name.toUtf8().constData());

    if (fs::exists(databaseName))
        throw Exceptions::RuntimeError(
                    QStringLiteral("SQLite database '%1' already exists in %2().")
                    .arg(QString::fromStdString(databaseName.string()), __tiny_func__));

    std::ofstream fs(databaseName, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!fs.fail())
        return std::nullopt;

    throw Exceptions::RuntimeError(
                QStringLiteral("Creation of SQLite database '%1' failed in %2().")
                .arg(QString::fromStdString(databaseName.string()), __tiny_func__));
}

std::optional<QSqlQuery>
SQLiteSchemaBuilder::dropDatabaseIfExists(const QString &name) const
{
    const fspath databasePath(name.toUtf8().constData());

    if (fs::exists(databasePath))
        fs::remove(databasePath);

    return std::nullopt;
}

// TEST schema, test in functional tests silverqx
void SQLiteSchemaBuilder::dropAllTables() const
{
    if (m_connection.getDatabaseName() != QStringLiteral(":memory:")) {
        refreshDatabaseFile();
        return;
    }

    const auto &sqliteGrammar =
            dynamic_cast<const Grammars::SQLiteSchemaGrammar &>(m_grammar);

    m_connection.selectFromWriteConnection(sqliteGrammar.compileEnableWriteableSchema());

    m_connection.selectFromWriteConnection(sqliteGrammar.compileDropAllTables({}));

    m_connection.selectFromWriteConnection(sqliteGrammar.compileDisableWriteableSchema());

    m_connection.selectFromWriteConnection(sqliteGrammar.compileRebuild());
}

// TEST schema, test in functional tests silverqx
void SQLiteSchemaBuilder::dropAllViews() const
{
    const auto &sqliteGrammar =
            dynamic_cast<const Grammars::SQLiteSchemaGrammar &>(m_grammar);

    m_connection.selectFromWriteConnection(sqliteGrammar.compileEnableWriteableSchema());

    m_connection.selectFromWriteConnection(sqliteGrammar.compileDropAllViews({}));

    m_connection.selectFromWriteConnection(sqliteGrammar.compileDisableWriteableSchema());

    m_connection.selectFromWriteConnection(sqliteGrammar.compileRebuild());
}

QSqlQuery SQLiteSchemaBuilder::getAllTables() const
{
    // TODO schema, use postproccessor processColumnListing() silverqx
    return m_connection.selectFromWriteConnection(m_grammar.compileGetAllTables());
}

QSqlQuery SQLiteSchemaBuilder::getAllViews() const
{
    return m_connection.selectFromWriteConnection(m_grammar.compileGetAllViews());
}

void SQLiteSchemaBuilder::refreshDatabaseFile() const
{
    const auto &databaseName = m_connection.getDatabaseName();

    std::ofstream fs(databaseName.toUtf8().constData(),
                     std::ios::out | std::ios::binary | std::ios::trunc);

    if (!fs.fail())
        return;

    throw Exceptions::RuntimeError(
                QStringLiteral("Truncation of SQLite database '%1' failed in %2().")
                .arg(databaseName, __tiny_func__));
}

} // namespace Orm::SchemaNs

TINYORM_END_COMMON_NAMESPACE
