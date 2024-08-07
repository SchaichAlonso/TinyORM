#pragma once
#ifndef TOM_COMMANDS_COMPLETECOMMAND_HPP
#define TOM_COMMANDS_COMPLETECOMMAND_HPP

#include <orm/macros/systemheader.hpp>
TINY_SYSTEM_HEADER

#include <unordered_set>

#include "tom/commands/command.hpp"
#include "tom/tomconstants.hpp"

TINYORM_BEGIN_COMMON_NAMESPACE

namespace Tom::Commands
{

    /*! Complete command names and parameters (for tab-completion only). */
    class CompleteCommand : public Command
    {
        Q_DISABLE_COPY_MOVE(CompleteCommand)

    public:
        /*! Constructor. */
        CompleteCommand(Application &application, QCommandLineParser &parser);
        /*! Virtual destructor. */
        ~CompleteCommand() override = default;

        /*! The console command name. */
        inline QString name() const override;
        /*! The console command description. */
        inline QString description() const override;

        /*! The signature of the console command. */
        QList<CommandLineOption> optionsSignature() const override;

        /*! Execute the console command. */
        int run() override;

    protected:
#ifdef _MSC_VER
        /*! Currently processed tom command. */
        static std::optional<QString>
        getCurrentTomCommand(const QStringList &commandlineArgSplitted);
#else
        /*! Currently processed tom command. */
        static std::optional<QString>
        getCurrentTomCommand(const QString &commandlineArg, QString::size_type cword);
#endif
        /*! Get the command-line option value for --word= option (workaround for pwsh). */
        QString getWordOptionValue(
                const QStringList &commandlineArgSplitted,
                QString::size_type positionArg, QString::size_type commandlineArgSize);

        /*! Print all guessed commands. */
        int printGuessedCommands(
                const std::vector<std::shared_ptr<Command>> &commands) const;
        /*! Print all or guessed namespace names for the list command. */
        int printGuessedNamespaces(const QString &wordArg) const;
        /*! Print all or guessed shell names for the integrate command. */
        int printGuessedShells(const QString &wordArg) const;
#ifdef _MSC_VER
        /*! Print all or guessed section names for the about command --only= option. */
        int printGuessedSectionNamesForAbout(QStringView sectionNamesArg) const;
        /*! Print all or guessed connection names for the --database= option. */
        int printGuessedConnectionNames(const QString &connectionNamesArg) const;
        /*! Print all or guessed environment names for the --env= option. */
        int printGuessedEnvironmentNames(const QString &environmentNameArg) const;
#endif
        /*! Print all or guessed long option parameter names. */
        int printGuessedLongOptions(const std::optional<QString> &currentCommand,
                                    const QString &wordArg) const;
        /*! Print all or guessed short option parameter names. */
        int printGuessedShortOptions(const std::optional<QString> &currentCommand) const;

        /*! Get an option's default value part. */
        static QString getOptionDefaultValue(const QCommandLineOption &option);

        /*! Get options signature for the given command. */
        QList<CommandLineOption>
        getCommandOptionsSignature(const std::optional<QString> &command) const;

        /*! Option type (long/short). */
        enum struct OptionType : quint8
        {
            /*! Consider both long and short option arguments. */
            ANY,
            /*! Long option argument. */
            LONG,
            /*! Short option argument. */
            SHORT,
        };
        /*! Expose the OptionType enum. */
        using enum OptionType;

        /*! Determine whether the given word is an option argument. */
        static bool isOptionArgument(const QString &wordArg, OptionType type = ANY);
        /*! Determine whether the given word is a long option argument. */
        inline static bool isLongOption(const QString &wordArg);
        /*! Determine whether the given word is a short option argument. */
        inline static bool isShortOption(const QString &wordArg);
        /*! Determine if the given word is a long option argument with an array value. */
        inline static bool isLongOptionWithArrayValue(const QString &wordArg);
        /*! Get the command-line option value (eg. --database=value). */
        static QString getOptionValue(const QString &wordArg);

        /*! Determine whether the given command contains the given option. */
        bool commandHasLongOption(const QString &command, const QString &option);

        /*! Obtain all connection names tagged with shell:connection comment from
            the main.cpp file. */
        static QStringList getConnectionNamesFromFile();

#ifdef _MSC_VER
        /*! Return type for the initializePrintArrayOptionValues() method. */
        struct PrintArrayOptionValuesType
        {
            /*! Value to complete/find (passed on command-line). */
            QString lastOptionValueArg;
            /*! All values for completion (excluding already printed values). */
            QList<QStringView> allValuesFiltered;
            /*! Determine whether completing the first value (need by pwsh). */
            bool isFirstOptionValue;
            /*! Print all values (if the input option value argument is empty). */
            bool printAllValues;
        };

        /*! Initialize local variables for printing and guessing array option values. */
        static PrintArrayOptionValuesType
        initializePrintArrayOptionValues(QStringView optionValuesArg,
                                         const QStringList &allValues);
#endif

        /*! Commands to exclude from the list. */
        /*const*/ std::unordered_set<QString> m_dontList {
            Tom::Constants::Complete,
        };
    };

    /* public */

    QString CompleteCommand::name() const
    {
        return Constants::Complete;
    }

    QString CompleteCommand::description() const
    {
        return QStringLiteral(
                    "Complete command names and parameters (for tab-completion only)");
    }

    /* protected */

    bool CompleteCommand::isLongOption(const QString &wordArg)
    {
        return isOptionArgument(wordArg, LONG);
    }

    bool CompleteCommand::isShortOption(const QString &wordArg)
    {
        return isOptionArgument(wordArg, SHORT);
    }

} // namespace Tom::Commands

TINYORM_END_COMMON_NAMESPACE

#endif // TOM_COMMANDS_COMPLETECOMMAND_HPP
