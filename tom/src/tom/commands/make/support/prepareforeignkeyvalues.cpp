#include "tom/commands/make/support/prepareforeignkeyvalues.hpp"

#include <QCommandLineParser>

#include "tom/commands/make/modelcommand.hpp"

#ifndef TINYTOM_DEBUG
#  include "tom/exceptions/runtimeerror.hpp"
#endif

TINYORM_BEGIN_COMMON_NAMESPACE

using Tom::Constants::belongs_to;
using Tom::Constants::belongs_to_many;
using Tom::Constants::foreign_key;
using Tom::Constants::one_to_many;
using Tom::Constants::one_to_one;

namespace Tom::Commands::Make::Support
{

/* public */

PrepareForeignKeyValues::PrepareForeignKeyValues(ModelCommand &modelCommand,
                                                 QStringList &&values)
    : m_modelCommand(modelCommand)
    , m_values(std::move(values))
{}

ForeignKeys PrepareForeignKeyValues::prepareValues()
{
    auto optionNames = modelCommand().optionNames();

    // Nothing to prepare
    if (optionNames.isEmpty())
        return {};

    for (auto &&option : optionNames) {
        // Try to start a new relation during foreign key names search
        if (startNewRelation(option))
            continue;

        // Searching a foreign key name after the relation option on the command-line
        // Nothing to do
        if (option != foreign_key)
            continue;

        /* Foreign key name defined before a relation or more options given for one
           relation. */
        if (isUnusedForeignKeyOption())
            continue;

        // Foreign key name found, assign it to the correct relation type
        insertForeignKeyValue();
    }

    // Handle the last relation
    // No foreign key name was passed on the command-line so insert the default value
    if (!m_currentRelation.isEmpty() &&
        !m_wasForeignKeySet && !m_wasForeignKeySetPartial
    )
        insertEmptyForeignKeyValue();

    return m_preparedValues;
}

/* private */

bool PrepareForeignKeyValues::startNewRelation(QString &option)
{
    // Nothing to start
    if (!ModelCommand::relationNames().contains(option))
        return false;

    // Relation option passed on the command-line, start (found relation option)

    /* No foreign key name was passed on the command-line so insert the default value.
       Don't insert the default value if the first relation have not been found. */
    if (!m_currentRelation.isEmpty() &&
        !m_wasForeignKeySet && !m_wasForeignKeySetPartial
    )
        insertEmptyForeignKeyValue();

    m_currentRelation = std::move(option);

    // Reset to defaults
    m_wasForeignKeySet = false;
    m_wasForeignKeySetPartial = false;

    return true;
}

bool PrepareForeignKeyValues::isUnusedForeignKeyOption()
{
    /* m_currentRelation not empty means that any relation was defined
       on the command-line and a value was not set yet. */
    if (!m_currentRelation.isEmpty() && !m_wasForeignKeySet)
        return false;

    /* Foreign key name defined before a relation or more options given for one
       relation. */

    showUnusedForeignKeyWarning();

    // Skip the value, only the first option's value is used
    if (m_wasForeignKeySet)
        ++m_valueIndex;

    return true;
}

void PrepareForeignKeyValues::insertEmptyForeignKeyValue()
{
    if (m_currentRelation == one_to_one)
        m_preparedValues.oneToOne.push_back({});

    else if (m_currentRelation == one_to_many)
        m_preparedValues.oneToMany.push_back({});

    else if (m_currentRelation == belongs_to)
        m_preparedValues.belongsTo.push_back({});

    else if (m_currentRelation == belongs_to_many)
#if defined(__clang__) && __clang_major__ < 16
        m_preparedValues.belongsToMany.push_back({}); // NOLINT(modernize-use-emplace)
#else
        m_preparedValues.belongsToMany.emplace_back();
#endif
    else
#ifndef TINYTOM_DEBUG
        throw Exceptions::RuntimeError(u"Unexpected value for m_currentRelation."_s);
#else
        Q_UNREACHABLE();
#endif
}

void PrepareForeignKeyValues::insertForeignKeyValue()
{
    if (m_currentRelation == one_to_one) {
        m_preparedValues.oneToOne << m_values.at(m_valueIndex++);
        m_wasForeignKeySet = true;
    }
    else if (m_currentRelation == one_to_many) {
        m_preparedValues.oneToMany << m_values.at(m_valueIndex++);
        m_wasForeignKeySet = true;
    }
    else if (m_currentRelation == belongs_to) {
        m_preparedValues.belongsTo << m_values.at(m_valueIndex++);
        m_wasForeignKeySet = true;
    }
    else if (m_currentRelation == belongs_to_many)
        insertForeignKeyValueBtm();

    else
#ifndef TINYTOM_DEBUG
        throw Exceptions::RuntimeError(u"Unexpected value for m_currentRelation."_s);
#else
        Q_UNREACHABLE();
#endif
}

void PrepareForeignKeyValues::insertForeignKeyValueBtm()
{
    Q_ASSERT(m_preparedValues.belongsToMany.size() <= 2);

    /* Both foreign key names can be assigned using one --foreign-key= option, in this
       case they must be divided by the , character.
       Or they can be assigned by two --foreign-key= options.
       If only one value was passed then set the related pivot key first. */

    const auto &value = m_values.at(m_valueIndex++);

    // First value
    if (!m_wasForeignKeySetPartial) {
#if defined(__clang__) && __clang_major__ < 16
        m_preparedValues.belongsToMany.push_back({{}, value});
#else
        m_preparedValues.belongsToMany.emplace_back<QString>({}, value);
#endif

        m_wasForeignKeySetPartial = true;

        return;
    }

    // Second value
    /* Swap pivot keys, so it follows belongsToMany() parameters order.
       It relates to this: if only one value was passed then set the related pivot key
       first, what means that during second value insertion values have to be swapped. */
    // Last record (we are currently processing)
    auto &last = m_preparedValues.belongsToMany.back();

    last.foreignPivotKey.swap(last.relatedPivotKey);
    last.relatedPivotKey = value;

    m_wasForeignKeySet = true;
}

void PrepareForeignKeyValues::showUnusedForeignKeyWarning()
{
    // Already shown
    if (modelCommand().m_shownUnusedForeignKey)
        return;

    modelCommand().comment(
                u"Unused --foreign-key= option, it has to follow after any relation "
                 "option and should be defined only once for every relation option "
                 "or can be defined twice for the --belongs-to-many= option."_s);

    modelCommand().m_shownUnusedForeignKey = true;
}

} // namespace Tom::Commands::Make::Support

TINYORM_END_COMMON_NAMESPACE
