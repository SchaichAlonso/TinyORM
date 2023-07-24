#include "tom/commands/migrations/freshcommand.hpp"

#include <QCommandLineParser>

#include <orm/constants.hpp>

#include "tom/migrator.hpp"

/*! Alias for the QStringLiteral(). */
#define sl(str) QStringLiteral(str)

TINYORM_BEGIN_COMMON_NAMESPACE

using Orm::Constants::database_;

using Tom::Constants::class_;
using Tom::Constants::database_up;
using Tom::Constants::drop_types;
using Tom::Constants::drop_views;
using Tom::Constants::force;
using Tom::Constants::seed;
using Tom::Constants::seeder;
using Tom::Constants::seeder_up;
using Tom::Constants::step_;
using Tom::Constants::DbSeed;
using Tom::Constants::DbWipe;
using Tom::Constants::Migrate;

namespace Tom::Commands::Migrations
{

/* Don't add the --pretend option for the migrate:fresh command. It doesn't make sense
   to have it because it doesn't show relevant informations as we can't really call
   the db:wipe command, so it would show pretended queries on the base of the current
   state of the migration repository table and not all pretended queries for all
   migrations.
   So eg. if the DB would be fully migrated and the migrate:fresh --pretend would be
   called, then the output would be empty, but it would be logical to show all pretended
   queries for all migrations. 🤯🙃😵‍💫 */

/* public */

FreshCommand::FreshCommand(
        Application &application, QCommandLineParser &parser,
        std::shared_ptr<Migrator> migrator
)
    : Command(application, parser)
    , Concerns::UsingConnection(connectionResolver())
    , m_migrator(std::move(migrator))
{}

QList<CommandLineOption> FreshCommand::optionsSignature() const
{
    return {
        {database_,     sl("The database connection to use <comment>(multiple values "
                           "allowed)</comment>"), database_up}, // Value
        {drop_views,    sl("Drop all tables and views")},
        {drop_types,    sl("Drop all tables and types (Postgres only)")},
        {{QChar('f'),
          force},       sl("Force the operation to run when in production")},
//        {"schema-path", sl("The path to a schema dump file")}, // Value
        {seed,          sl("Indicates if the seed task should be re-run")},
        {seeder,        sl("The class name of the root seeder"), seeder_up}, // Value
        {step_,         sl("Force the migrations to be run so they can be rolled back "
                           "individually")},
    };
}

int FreshCommand::run()
{
    Command::run();

    // Ask for confirmation in the production environment
    if (!confirmToProceed())
        return EXIT_FAILURE;

    // Database connection to use (multiple connections supported)
    return usingConnections(values(database_), isDebugVerbosity(),
                            [this](const QString &database)
    {
        auto databaseCmd = longOption(database_, database);

        int exitCode = EXIT_SUCCESS; // NOLINT(misc-const-correctness)

        exitCode |= call(DbWipe, {databaseCmd,
                                  longOption(force),
                                  boolCmd(drop_views),
                                  boolCmd(drop_types)});

        exitCode |= call(Migrate, {databaseCmd,
                                   longOption(force),
                                   boolCmd(step_)});
//                                   valueCmd("schema-path")});

        // Invoke seeder
        if (needsSeeding())
            exitCode |= runSeeder(std::move(databaseCmd));

        // Return success only, if all executed commands were successful
        return exitCode == EXIT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
    });
}

/* protected */

bool FreshCommand::needsSeeding() const
{
    return isSet(seed) || !value(seeder).isEmpty();
}

int FreshCommand::runSeeder(QString &&databaseCmd) const
{
    return call(DbSeed, {std::move(databaseCmd),
                         longOption(force),
                         valueCmd(seeder, class_)});
}

} // namespace Tom::Commands::Migrations

TINYORM_END_COMMON_NAMESPACE
