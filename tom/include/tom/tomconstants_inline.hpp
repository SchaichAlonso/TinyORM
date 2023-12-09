#pragma once
#ifndef TOM_TOMCONSTANTS_INLINE_HPP
#define TOM_TOMCONSTANTS_INLINE_HPP

#include <orm/macros/systemheader.hpp>
TINY_SYSTEM_HEADER

#include <QString>

#include <orm/macros/commonnamespace.hpp>

TINYORM_BEGIN_COMMON_NAMESPACE

/*! Namespace contains common strings used in the Tom namespace (migrations). */
namespace Tom::Constants
{

    /*! Migration files datetime prefix format. */
    inline const QString DateTimePrefix  = QStringLiteral("yyyy_MM_dd_HHmmss");

    // Common strings
    inline const QString LongOption      = QStringLiteral("--%1");
    inline const QString LongOptionOnly  = QStringLiteral("    %1");
    inline const QString LongOptionValue = QStringLiteral("--%1[=%2]");
    inline const QString ShortOption     = QStringLiteral("-%1");
    inline const QString DoubleDash      = QStringLiteral("--");

    inline const QString migration_  = QStringLiteral("migration");
    inline const QString batch_      = QStringLiteral("batch");

    // Common options
    inline const QString ansi          = QStringLiteral("ansi");
    inline const QString noansi        = QStringLiteral("no-ansi");
    inline const QString env           = QStringLiteral("env");
    inline const QString help          = QStringLiteral("help");
    inline const QString nointeraction = QStringLiteral("no-interaction");
    inline const QString quiet         = QStringLiteral("quiet");
    inline const QString verbose       = QStringLiteral("verbose");

    // Commands' positional options
    // help
    inline const QString command_name = QStringLiteral("command_name");
    // list
    inline const QString namespace_   = QStringLiteral("namespace");
    inline const QString shell_       = QStringLiteral("shell");

    // Commands' options
    // Used by more commands
    inline const QString force        = QStringLiteral("force");
    inline const QString pretend      = QStringLiteral("pretend");
    inline const QString seed         = QStringLiteral("seed");
    inline const QString seeder       = QStringLiteral("seeder");
    inline const QString step_        = QStringLiteral("step");
    inline const QString path_        = QStringLiteral("path");
    inline const QString json_        = QStringLiteral("json");
    inline const QString pretty       = QStringLiteral("pretty");
    inline const QString only_        = QStringLiteral("only");
    // Default value names
    inline const QString env_up             = QStringLiteral("ENV");
    inline const QString class_up           = QStringLiteral("CLASS");
    inline const QString database_up        = QStringLiteral("DATABASE");
    inline const QString seeder_up          = QStringLiteral("SEEDER");
    inline const QString batch_up           = QStringLiteral("BATCH");
    inline const QString step_up            = QStringLiteral("STEP");
    inline const QString commandline_up     = QStringLiteral("COMMANDLINE");
    inline const QString position_up        = QStringLiteral("POSITION");
    inline const QString word_up            = QStringLiteral("WORD");
    inline const QString cword_up           = QStringLiteral("CWORD");
    inline const QString path_up            = QStringLiteral("PATH");
    inline const QString create_up          = QStringLiteral("CREATE");
    inline const QString table_up           = QStringLiteral("TABLE");
    inline const QString primary_key_up     = QStringLiteral("PRIMARYKEY");
    inline const QString connection_up      = QStringLiteral("CONNECTION");
    inline const QString with_up            = QStringLiteral("WITH");
    inline const QString fillable_up        = QStringLiteral("FILLABLE");
    inline const QString guarded_up         = QStringLiteral("GUARDED");
    inline const QString dateformat_up      = QStringLiteral("DATEFORMAT");
    inline const QString dates_up           = QStringLiteral("DATES");
    inline const QString touches_up         = QStringLiteral("TOUCHES");
    inline const QString one_to_one_up      = QStringLiteral("OTO");
    inline const QString one_to_many_up     = QStringLiteral("OTM");
    inline const QString belongs_to_up      = QStringLiteral("BT");
    inline const QString belongs_to_many_up = QStringLiteral("BTM");
    inline const QString pivot_up           = QStringLiteral("PIVOT");
    inline const QString pivot_inverse_up   = QStringLiteral("PIVOTINVERSE");
    inline const QString as_up              = QStringLiteral("AS");
    inline const QString with_pivot_up      = QStringLiteral("WITHPIVOT");
    inline const QString foreign_key_up     = QStringLiteral("FOREIGNKEY");
    inline const QString pivot_table_up     = QStringLiteral("PIVOTTABLE");
    inline const QString visible_up         = QStringLiteral("VISIBLE");
    inline const QString hidden_up          = QStringLiteral("HIDDEN");
    inline const QString accessors_up       = QStringLiteral("ACCESSORS");
    inline const QString appends_up         = QStringLiteral("APPENDS");
    inline const QString only_up            = QStringLiteral("ONLY");
    // complete
    inline const QString commandline        = QStringLiteral("commandline");
    inline const QString position_          = QStringLiteral("position");
    inline const QString word_              = QStringLiteral("word");
    inline const QString cword_             = QStringLiteral("cword");
    // list
    inline const QString raw_               = QStringLiteral("raw");
    // db:seed
    inline const QString class_             = QStringLiteral("class");
    // db:wipe
    inline const QString drop_views         = QStringLiteral("drop-views");
    inline const QString drop_types         = QStringLiteral("drop-types");
    // make:migration, make:model, make:seeder
    inline const QString create_            = QStringLiteral("create");
    inline const QString table_             = QStringLiteral("table");
    inline const QString realpath_          = QStringLiteral("realpath");
    inline const QString fullpath           = QStringLiteral("fullpath");
    inline const QString from_model         = QStringLiteral("from-model");
    // make:model
    inline const QString one_to_one           = QStringLiteral("one-to-one");
    inline const QString one_to_many          = QStringLiteral("one-to-many");
    inline const QString belongs_to           = QStringLiteral("belongs-to");
    inline const QString belongs_to_many      = QStringLiteral("belongs-to-many");
    inline const QString foreign_key          = QStringLiteral("foreign-key");
    inline const QString pivot_table          = QStringLiteral("pivot-table");
    inline const QString primary_key          = QStringLiteral("primary-key");
    inline const QString incrementing         = QStringLiteral("incrementing");
    inline const QString disable_incrementing = QStringLiteral("disable-incrementing");
    inline const QString connection_          = QStringLiteral("connection");
    inline const QString with_                = QStringLiteral("with");
    inline const QString fillable             = QStringLiteral("fillable");
    inline const QString guarded              = QStringLiteral("guarded");
    inline const QString disable_timestamps   = QStringLiteral("disable-timestamps");
    inline const QString dateformat           = QStringLiteral("dateformat");
    inline const QString dates                = QStringLiteral("dates");
    inline const QString touches              = QStringLiteral("touches");
    inline const QString pivot_model          = QStringLiteral("pivot-model");
    inline const QString pivot_inverse        = QStringLiteral("pivot-inverse");
    inline const QString as_                  = QStringLiteral("as");
    inline const QString with_timestamps      = QStringLiteral("with-timestamps");
    inline const QString with_pivot           = QStringLiteral("with-pivot");
    inline const QString preserve_order       = QStringLiteral("preserve-order");
    inline const QString casts_example        = QStringLiteral("casts-example");
    inline const QString snake_attributes     = QStringLiteral("snake-attributes");
    inline const QString
    disable_snake_attributes                  = QStringLiteral("disable-snake-attributes");
    inline const QString visible              = QStringLiteral("visible");
    inline const QString hidden               = QStringLiteral("hidden");
    inline const QString accessors            = QStringLiteral("accessors");
    inline const QString appends              = QStringLiteral("appends");
    // migrate:refresh
    inline const QString step_migrate         = QStringLiteral("step-migrate");
    // migrate:status
    inline const QString pending_             = QStringLiteral("pending");
    // migrate:uninstall
    inline const QString reset                = QStringLiteral("reset");
    // integrate
    inline const QString stdout_              = QStringLiteral("stdout");

    // Namespace names
    inline const QString NsGlobal     = QStringLiteral("global");
    inline const QString NsDb         = QStringLiteral("db");
    inline const QString NsMake       = QStringLiteral("make");
    inline const QString NsMigrate    = QStringLiteral("migrate");
    inline const QString NsNamespaced = QStringLiteral("namespaced");
    inline const QString NsAll        = QStringLiteral("all");

    // Shell names
    inline const QString ShBash       = QStringLiteral("bash");
    inline const QString ShPwsh       = QStringLiteral("pwsh");
    inline const QString ShZsh        = QStringLiteral("zsh");

    // Command names
    inline const QString About            = QStringLiteral("about");
    inline const QString Complete         = QStringLiteral("complete");
    inline const QString DbSeed           = QStringLiteral("db:seed");
    inline const QString DbWipe           = QStringLiteral("db:wipe");
    inline const QString Inspire          = QStringLiteral("inspire");
    inline const QString List             = QStringLiteral("list");
    inline const QString &Env             = Tom::Constants::env;
    inline const QString &Help            = Tom::Constants::help;
    inline const QString MakeMigration    = QStringLiteral("make:migration");
    inline const QString MakeModel        = QStringLiteral("make:model");
    inline const QString MakeProject      = QStringLiteral("make:project");
    inline const QString MakeSeeder       = QStringLiteral("make:seeder");
    inline const QString &Migrate         = Tom::Constants::NsMigrate;
    inline const QString MigrateFresh     = QStringLiteral("migrate:fresh");
    inline const QString MigrateInstall   = QStringLiteral("migrate:install");
    inline const QString MigrateRollback  = QStringLiteral("migrate:rollback");
    inline const QString MigrateRefresh   = QStringLiteral("migrate:refresh");
    inline const QString MigrateReset     = QStringLiteral("migrate:reset");
    inline const QString MigrateStatus    = QStringLiteral("migrate:status");
    inline const QString MigrateUninstall = QStringLiteral("migrate:uninstall");
    inline const QString Integrate        = QStringLiteral("integrate");

} // namespace Tom::Constants

TINYORM_END_COMMON_NAMESPACE

#endif // TOM_TOMCONSTANTS_INLINE_HPP
