#include "tom/commands/migrations/migratecommand.hpp"

#include <QCommandLineParser>

#include <orm/constants.hpp>

#include "tom/migrator.hpp"

TINYORM_BEGIN_COMMON_NAMESPACE

using Orm::Constants::database_;

using Tom::Constants::database_up;
using Tom::Constants::force;
using Tom::Constants::pretend;
using Tom::Constants::seed;
using Tom::Constants::step_;
using Tom::Constants::DbSeed;
using Tom::Constants::MigrateInstall;

namespace Tom::Commands::Migrations
{

/* public */

MigrateCommand::MigrateCommand(
        Application &application, QCommandLineParser &parser,
        std::shared_ptr<Migrator> migrator
)
    : Command(application, parser)
    , Concerns::UsingConnection(connectionResolver())
    , m_migrator(std::move(migrator))
{}

QList<CommandLineOption> MigrateCommand::optionsSignature() const
{
    return {
        {database_,     u"The database connection to use <comment>(multiple values "
                         "allowed)</comment>"_s, database_up}, // Value
        {{QChar('f'),
          force},       u"Force the operation to run when in production"_s},
        {pretend,       u"Dump the SQL queries that would be run"_s},
//        {"schema-path", u"The path to a schema dump file"_s}, // Value
        {seed,          u"Indicates if the seed task should be re-run"_s},
        {step_,         u"Force the migrations to be run so they can be rolled back "
                         "individually"_s},
    };
}

int MigrateCommand::run()
{
    Command::run();

    // Ask for confirmation in the production environment
    if (!confirmToProceed())
        return EXIT_FAILURE;

    // Database connection to use (multiple connections supported)
    return usingConnections(
                values(database_), isDebugVerbosity(), m_migrator->repository(),
                [this](const auto &database)
    {
        // Install db repository and load schema state
        prepareDatabase(database);

        /* Next, we will check to see if a path option has been defined. If it has
               we will use the path relative to the root of this installation folder
               so that migrations may be run for any path within the applications. */
        m_migrator->run({isSet(pretend), isSet(step_)});

        info(u"Database migration completed successfully."_s);

        int exitCode = EXIT_SUCCESS; // NOLINT(misc-const-correctness)

        /* Finally, if the "seed" option has been given, we will re-run the database
           seed task to re-populate the database, which is convenient when adding
           a migration and a seed at the same time, as it is only this command. */
        if (isSet(seed))
            exitCode |= runSeeder(database);

        // Return success only, if all executed commands were successful
        return exitCode == EXIT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
    });
}

/* protected */

void MigrateCommand::prepareDatabase(const QString &database) const
{
    /* Don't proxy the pretend option here because the Migrator needs access to
       the migration repository table, on the base of it it computes migrations
       to execute, so it can also correctly show pretended queries.
       It would be possible to make it, but every Migrator operation run(), rollback(),
       reset() would have to also check if the migration table exists and return empty
       results set, I don't like it this way. Currently with the pretend proxying
       it throws an exception during querying the migration repository because
       it doesn't exist. */
    if (!m_migrator->repositoryExists())
        call(MigrateInstall, {longOption(database_, database)});

//    if (!m_migrator->hasRunAnyMigrations() && !isSet(pretend))
//        loadSchemaState();
}

//void MigrateCommand::loadSchemaState() const
//{
//    // CUR tom, finish load schema silverqx
//}

int MigrateCommand::runSeeder(const QString &database) const
{
    return call(DbSeed, {longOption(database_, database),
                         longOption(force),
                         boolCmd(pretend)});
}

} // namespace Tom::Commands::Migrations

TINYORM_END_COMMON_NAMESPACE
