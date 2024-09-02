#include "tom/commands/migrations/uninstallcommand.hpp"

#include <QCommandLineParser>

#include <orm/constants.hpp>

#include "tom/migrationrepository.hpp"

TINYORM_BEGIN_COMMON_NAMESPACE

using namespace Qt::StringLiterals; // NOLINT(google-build-using-namespace)

using Orm::Constants::database_;

using Tom::Constants::MigrateReset;
using Tom::Constants::database_up;
using Tom::Constants::force;
using Tom::Constants::pretend;
using Tom::Constants::reset;

namespace Tom::Commands::Migrations
{

/* public */

UninstallCommand::UninstallCommand(
        Application &application, QCommandLineParser &parser,
        std::shared_ptr<MigrationRepository> repository
)
    : Command(application, parser)
    , Concerns::UsingConnection(connectionResolver())
    , m_repository(std::move(repository))
{}

QList<CommandLineOption> UninstallCommand::optionsSignature() const
{
    return {
        {database_,   u"The database connection to use <comment>(multiple values "
                       "allowed)</comment>"_s, database_up}, // Value
        {{QChar('f'),
          force},     u"Force the operation to run when in production"_s},
        {pretend,     u"Dump the SQL queries that would be run"_s},
        {{QChar('r'),
          reset},     u"Rollback all database migrations"_s},
    };
}

int UninstallCommand::run()
{
    Command::run();

    // Ask for confirmation in the production environment
    if (!confirmToProceed())
        return EXIT_FAILURE;

    // Database connection to use (multiple connections supported)
    return usingConnections(values(database_), isDebugVerbosity(), *m_repository,
                            [this](const QString &database)


    {
        int exitCode = EXIT_SUCCESS; // NOLINT(misc-const-correctness)

        // Call the migrate:reset if the --reset option was set
        if (isSet(reset))
            exitCode |= call(MigrateReset, {valueCmd(database_),
                                            longOption(force),
                                            boolCmd(pretend)});

        // Uninstall the migration database repository
        optionalPretend(isSet(pretend), database, [this]
        {
            m_repository->dropRepositoryIfExists();
        });

        info(u"Migration table dropped successfully."_s);

        // Return success only, if all executed commands were successful
        return exitCode == EXIT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
    });
}

} // namespace Tom::Commands::Migrations

TINYORM_END_COMMON_NAMESPACE
