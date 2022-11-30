#include "tom/commands/migrations/installcommand.hpp"

#include <QCommandLineParser>

#include <orm/constants.hpp>

#include "tom/migrationrepository.hpp"

TINYORM_BEGIN_COMMON_NAMESPACE

using Orm::Constants::database_;

using Tom::Constants::database_up;

namespace Tom::Commands::Migrations
{

/* public */

InstallCommand::InstallCommand(
        Application &application, QCommandLineParser &parser,
        std::shared_ptr<MigrationRepository> repository
)
    : Command(application, parser)
    , Concerns::UsingConnection(connectionResolver())
    , m_repository(std::move(repository))
{}

QList<CommandLineOption> InstallCommand::optionsSignature() const
{
    return {
        {database_, QStringLiteral("The database connection to use "
                                   "<comment>(multiple values allowed)</comment>"),
                    database_up}, // Value
    };
}

int InstallCommand::run()
{
    Command::run();

    // Database connection to use (multiple connections supported)
    return usingConnections(values(database_), isDebugVerbosity(), *m_repository,
                            [this]
    {
        m_repository->createRepository();

        info(QStringLiteral("Migration table created successfully."));

        return EXIT_SUCCESS;
    });
}

} // namespace Tom::Commands::Migrations

TINYORM_END_COMMON_NAMESPACE

// FUTURE tom, add migrate:uninstall or migrate:install --uninstall silverqx
