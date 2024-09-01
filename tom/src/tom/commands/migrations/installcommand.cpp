#include "tom/commands/migrations/installcommand.hpp"

#include <QCommandLineParser>

#include <orm/constants.hpp>

#include "tom/migrationrepository.hpp"

TINYORM_BEGIN_COMMON_NAMESPACE

using namespace Qt::StringLiterals; // NOLINT(google-build-using-namespace)

using Orm::Constants::database_;

using Tom::Constants::database_up;
using Tom::Constants::force;
using Tom::Constants::pretend;

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
        {database_,   u"The database connection to use <comment>(multiple values "
                       "allowed)</comment>"_s, database_up}, // Value
        {{QChar('f'),
          force},     u"Force the operation to run when in production"_s},
        {pretend,     u"Dump the SQL queries that would be run"_s},
    };
}

int InstallCommand::run()
{
    Command::run();

    // Ask for confirmation in the production environment
    if (!confirmToProceed())
        return EXIT_FAILURE;

    // Database connection to use (multiple connections supported)
    return usingConnections(values(database_), isDebugVerbosity(), *m_repository,
                            [this](const QString &database)
    {
        // Install the migration database repository
        optionalPretend(isSet(pretend), database, [this]
        {
            m_repository->createRepository();
        });

        info(QStringLiteral("Migration table created successfully."));

        return EXIT_SUCCESS;
    });
}

} // namespace Tom::Commands::Migrations

TINYORM_END_COMMON_NAMESPACE
