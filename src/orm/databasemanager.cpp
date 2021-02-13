#include "orm/databasemanager.hpp"

#include "orm/concerns/hasconnectionresolver.hpp"
#include "orm/databaseconnection.hpp"

#ifdef TINYORM_COMMON_NAMESPACE
namespace TINYORM_COMMON_NAMESPACE
{
#endif
namespace Orm
{

DatabaseManager *DatabaseManager::m_instance = nullptr;
const char *DatabaseManager::defaultConnectionName = const_cast<char *>("tinyorm_default");

/* This is needed because of the std::unique_ptr is used in the m_connections
   data member 😲, and when this dtor is not defined in the cpp, it will be
   generated by the compiler as inline dtor, what cause a compile error. */
DatabaseManager::~DatabaseManager() = default;

// TODO future add support for ::read and ::write db connections silverx
// TODO implement RepositoryFactory silverqx
DatabaseManager::DatabaseManager(const QString &defaultConnection)
{
    m_config.defaultConnection = defaultConnection;

    // Set up the DatabaseManager as a connection resolver
    Concerns::HasConnectionResolver::setConnectionResolver(this);

    setupDefaultReconnector();

    // TODO prevent more instances??, or eg add allowMoreInstances data member to override this behavior (or allowMoreInstances compile directive?, in that case would be possible to use #ifdef for ::instance() static method 🤔) silverqx
    m_instance = this;
}

DatabaseManager::DatabaseManager(const QVariantHash &config, const QString &name,
                                 const QString &defaultConnection)
    : DatabaseManager(defaultConnection)
{
    addConnection(config, name);
}

DatabaseManager::DatabaseManager(const ConfigurationsType &configs,
                                 const QString &defaultConnection)
    : DatabaseManager(defaultConnection)
{
    m_config.connections = configs;
}

DatabaseManager &DatabaseManager::setupDefaultReconnector()
{
    m_reconnector = [this](const DatabaseConnection &connection)
    {
        reconnect(connection.getName());
    };

    return *this;
}

std::unique_ptr<DatabaseManager>
DatabaseManager::create(const QVariantHash &config, const QString &connection)
{
    return std::unique_ptr<DatabaseManager>(
                new DatabaseManager(config, connection, connection));
}

std::unique_ptr<DatabaseManager>
DatabaseManager::create(const ConfigurationsType &configs,
                        const QString &defaultConnection)
{
    return std::unique_ptr<DatabaseManager>(
                new DatabaseManager(configs, defaultConnection));
}

QSharedPointer<QueryBuilder>
DatabaseManager::table(
        const QString &table, const QString &as, const QString &connection)
{
    return this->connection(connection).table(table, as);
}

QSharedPointer<QueryBuilder>
DatabaseManager::query(const QString &connection)
{
    return this->connection(connection).query();
}

QSqlQuery DatabaseManager::qtQuery(const QString &connection)
{
    return this->connection(connection).getQtQuery();
}

std::tuple<bool, QSqlQuery>
DatabaseManager::select(const QString &query, const QVector<QVariant> &bindings)
{
    return connection().select(query, bindings);
}

std::tuple<bool, QSqlQuery>
DatabaseManager::selectOne(const QString &query, const QVector<QVariant> &bindings)
{
    return connection().selectOne(query, bindings);
}

std::tuple<bool, QSqlQuery>
DatabaseManager::insert(const QString &query, const QVector<QVariant> &bindings)
{
    return connection().insert(query, bindings);
}

std::tuple<int, QSqlQuery>
DatabaseManager::update(const QString &query, const QVector<QVariant> &bindings)
{
    return connection().update(query, bindings);
}

std::tuple<int, QSqlQuery>
DatabaseManager::remove(const QString &query, const QVector<QVariant> &bindings)
{
    return connection().remove(query, bindings);
}

std::tuple<bool, QSqlQuery>
DatabaseManager::statement(const QString &query, const QVector<QVariant> &bindings)
{
    return connection().statement(query, bindings);
}

bool DatabaseManager::beginTransaction()
{
    return connection().beginTransaction();
}

bool DatabaseManager::commit()
{
    return connection().commit();
}

bool DatabaseManager::rollBack()
{
    return connection().rollBack();
}

bool DatabaseManager::savepoint(const QString &id)
{
    return connection().savepoint(id);
}

bool DatabaseManager::savepoint(const size_t id)
{
    return connection().savepoint(id);
}

bool DatabaseManager::rollbackToSavepoint(const QString &id)
{
    return connection().rollbackToSavepoint(id);
}

bool DatabaseManager::rollbackToSavepoint(const size_t id)
{
    return connection().rollbackToSavepoint(id);
}

uint DatabaseManager::transactionLevel()
{
    return connection().transactionLevel();
}

DatabaseManager *DatabaseManager::instance()
{
    if (!m_instance)
        throw OrmRuntimeError(
                "The DatabaseManager instance has not been created yet, create it "
                "by DB::create() method.");

    return m_instance;
}

ConnectionInterface &DatabaseManager::connection(const QString &name)
{
    const auto &connectionName = parseConnectionName(name);

    /* If we haven't created this connection, we'll create it based on the provided
       config. Once we've created the connections we will configure it. */
    m_connections.try_emplace(connectionName,
                              configure(makeConnection(connectionName)));

    return *m_connections[connectionName];
}

DatabaseManager &
DatabaseManager::addConnection(const QVariantHash &config, const QString &name)
{
    m_config.connections.insert(name, config);

    return *this;
}

ConnectionInterface &DatabaseManager::reconnect(QString name)
{
    if (name.isEmpty())
        name = getDefaultConnection();

    disconnect(name);

    if (!m_connections.contains(name))
        return connection(name);

    return refreshPdoConnections(name);
}

void DatabaseManager::disconnect(QString name) const
{
    if (name.isEmpty())
        name = getDefaultConnection();

    if (m_connections.contains(name))
        m_connections.find(name)->second->disconnect();
}

const QStringList DatabaseManager::supportedDrivers() const
{
    // TODO future add method to not only supported drivers, but also check if driver is available/loadable by qsqldatabase silverqx
    // aaaaaaaaaaaaaachjo 🤔😁
    return {"mysql"};
    //    return {"mysql", "pgsql", "sqlite", "sqlsrv"};
}

const QString &
DatabaseManager::getDefaultConnection() const
{
    return m_config.defaultConnection;
}

void DatabaseManager::setDefaultConnection(const QString &defaultConnection)
{
    m_config.defaultConnection = defaultConnection;
}

DatabaseManager &
DatabaseManager::setReconnector(const ReconnectorType &reconnector)
{
    m_reconnector = reconnector;

    return *this;
}

const QString &
DatabaseManager::parseConnectionName(const QString &name) const
{
    return name.isEmpty() ? getDefaultConnection() : name;
}

std::unique_ptr<DatabaseConnection>
DatabaseManager::makeConnection(const QString &name)
{
    auto &config = configuration(name);

    // TODO future add support for extensions silverqx

    return m_factory.make(config, name);
}

/* Can not be const because I'm modifying the Configuration (QVariantHash)
   in ConnectionFactory. */
QVariantHash &
DatabaseManager::configuration(QString name)
{
    if (name.isEmpty())
        name = getDefaultConnection();

    /* Get the database connection configuration by the given name.
       If the configuration doesn't exist, we'll throw an exception and bail. */
    if (!m_config.connections.contains(name))
        throw std::invalid_argument(
                "Database connection '" + name.toStdString() + "' not configured.");

    return m_config.connections[name];

    // TODO add ConfigurationUrlParser silverqx
//    return (new ConfigurationUrlParser)
//                ->parseConfiguration($config);
}

std::unique_ptr<DatabaseConnection>
DatabaseManager::configure(std::unique_ptr<DatabaseConnection> connection) const
{
    /* Here we'll set a reconnector lambda. This reconnector can be any callable
       so we will set a Closure to reconnect from this manager with the name of
       the connection, which will allow us to reconnect from OUR connections. */
    connection->setReconnector(m_reconnector);

    return connection;
}

DatabaseConnection &
DatabaseManager::refreshPdoConnections(const QString &name)
{
    /* Make OUR new connection and copy the connection resolver from this new
       connection to the current connection, this ensure that the connection
       will be resolved/connected again lazily. */
    auto fresh = makeConnection(name);

    return m_connections[name]->setQtConnectionResolver(
                fresh->getQtConnectionResolver());
}

} // namespace Orm
#ifdef TINYORM_COMMON_NAMESPACE
} // namespace TINYORM_COMMON_NAMESPACE
#endif
