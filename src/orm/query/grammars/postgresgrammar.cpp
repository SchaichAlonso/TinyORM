#include "orm/query/grammars/postgresgrammar.hpp"

#include "orm/query/querybuilder.hpp" // IWYU pragma: keep

TINYORM_BEGIN_COMMON_NAMESPACE

namespace Orm::Query::Grammars
{

/* public */

QString PostgresGrammar::compileInsertOrIgnore(const QueryBuilder &query,
                                               const QList<QVariantMap> &values) const
{
    return u"%1 on conflict do nothing"_s.arg(compileInsert(query, values));
}

QString PostgresGrammar::compileInsertGetId(const QueryBuilder &query,
                                            const QList<QVariantMap> &values,
                                            const QString &sequence) const
{
    return u"%1 returning %2"_s
            .arg(compileInsert(query, values),
                 wrap(sequence.isEmpty() ? ID : sequence));
}

QString PostgresGrammar::compileUpdate(QueryBuilder &query,
                                       const QList<UpdateItem> &values) const
{
    if (!query.getJoins().isEmpty() || query.getLimit() > -1)
        return compileUpdateWithJoinsOrLimit(query, values);

    return Grammar::compileUpdate(query, values);
}

QString PostgresGrammar::compileUpsert(
            QueryBuilder &query, const QList<QVariantMap> &values,
            const QStringList &uniqueBy, const QStringList &update) const
{
    auto sql = compileInsert(query, values);

    sql += u" on conflict (%1) do update set "_s.arg(columnize(uniqueBy));

    QStringList columns;
    columns.reserve(update.size());

    for (const auto &column : update)
        columns << u"%1 = %2"_s
                   .arg(wrap(column),
                        DOT_IN.arg(wrapValue(u"excluded"_s), wrap(column)));

    return NOSPACE.arg(sql, columns.join(COMMA));
}

QString PostgresGrammar::compileDelete(QueryBuilder &query) const
{
    if (!query.getJoins().isEmpty() || query.getLimit() > -1)
        return compileDeleteWithJoinsOrLimit(query);

    return Grammar::compileDelete(query);
}

std::unordered_map<QString, QList<QVariant>>
PostgresGrammar::compileTruncate(const QueryBuilder &query) const
{
    return {{u"truncate %1 restart identity cascade"_s.arg(wrapTable(query.getFrom())),
            {}}};
}

QString PostgresGrammar::compileLock(const QueryBuilder &query) const
{
    const auto &lock = query.getLock();

    if (!std::holds_alternative<QString>(lock))
        return std::get<bool>(lock) ? u"for update"_s : u"for share"_s;

    return std::get<QString>(lock);
}

const std::unordered_set<QString> &PostgresGrammar::getOperators() const
{
    static const std::unordered_set<QString> cachedOperators {
        EQ, LT, GT, LE, GE, NE, NE_,
        LIKE, NLIKE, u"between"_s, ILIKE, u"not ilike"_s,
        u"~"_s, B_AND, B_OR, u"#"_s, u"<<"_s, u">>"_s, u"<<="_s, u">>="_s,
        AND_, u"@>"_s, u"<@"_s, u"?"_s, u"?|"_s, u"?&"_s, OR_, MINUS, u"@?"_s, u"@@"_s,
              u"#-"_s,
        u"is distinct from"_s, u"is not distinct from"_s,
    };

    return cachedOperators;
}

QString PostgresGrammar::whereBasic(const WhereConditionItem &where) const
{
    if (!where.comparison.contains(LIKE, Qt::CaseInsensitive))
        return Grammar::whereBasic(where);

    return u"%1::text %2 %3"_s.arg(wrap(where.column),
                                   where.comparison,
                                   parameter(where.value));
}

/* protected */

const QList<Grammar::SelectComponentValue> &
PostgresGrammar::getCompileMap() const
{
    /* Needed, because some compileXx() methods are overloaded, this way I will capture
       'this' reference and the compileMethod rvalue reference in the following lambda
       and simply save std::function<> in the SelectComponentValue's compileMethod data
       member. */
    const auto bind = [](auto &&compileMethod)
    {
        return [compileMethod = std::forward<decltype (compileMethod)>(compileMethod)]
               (const Grammar &grammar, const QueryBuilder &query)
        {
            /* We can be at 100% sure that this is the PostgresGrammar instance because
               this method is virtual; used the reinterpret_cast<> to avoid useless
               and slower dynamic_cast<>. */
            return std::invoke(compileMethod,
                               reinterpret_cast<const PostgresGrammar &>(grammar), query); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
        };
    };

    // Pointers to compile methods, yes yes c++ 😂
    static const QList<SelectComponentValue> cached {
        {bind(&PostgresGrammar::compileAggregate),
         [](const auto &query) { return shouldCompileAggregate(query.getAggregate()); }},
        {bind(&PostgresGrammar::compileColumns),
         [](const auto &query) { return shouldCompileColumns(query); }},
        {bind(&PostgresGrammar::compileFrom),
         [](const auto &query) { return shouldCompileFrom(query.getFrom()); }},
        {bind(&PostgresGrammar::compileJoins),
         [](const auto &query) { return !query.getJoins().isEmpty(); }},
        {bind(&PostgresGrammar::compileWheres),
         [](const auto &query) { return !query.getWheres().isEmpty(); }},
        {bind(&PostgresGrammar::compileGroups),
         [](const auto &query) { return !query.getGroups().isEmpty(); }},
        {bind(&PostgresGrammar::compileHavings),
         [](const auto &query) { return !query.getHavings().isEmpty(); }},
        {bind(&PostgresGrammar::compileOrders),
         [](const auto &query) { return !query.getOrders().isEmpty(); }},
        {bind(&PostgresGrammar::compileLimit),
         [](const auto &query) { return query.getLimit() > -1; }},
        {bind(&PostgresGrammar::compileOffset),
         [](const auto &query) { return query.getOffset() > -1; }},
        {bind(&PostgresGrammar::compileLock),
         [](const auto &query) { return query.getLock().index() != 0; }},
    };

    return cached;
}

const Grammar::WhereMemFn &
PostgresGrammar::getWhereMethod(const WhereType whereType) const
{
    /* Needed, because some compileXx() methods are overloaded, this way I will capture
       'this' reference and the compileMethod rvalue reference in the following lambda
       and simply save std::function<> in the SelectComponentValue's compileMethod data
       member. */
    const auto bind = [](auto &&compileMethod)
    {
        return [compileMethod = std::forward<decltype (compileMethod)>(compileMethod)]
               (const Grammar &grammar, const WhereConditionItem &query)
        {
            /* We can be at 100% sure that this is the PostgresGrammar instance because
               this method is virtual; used the reinterpret_cast<> to avoid useless
               and slower dynamic_cast<>. */
            return std::invoke(compileMethod,
                               reinterpret_cast<const PostgresGrammar &>(grammar), query); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
        };
    };

    /* Pointers to a where member methods by whereType, yes yes c++. 😂
       An order has to be the same as in enum struct WhereType.
       QList is ideal for this as we lookup using the index. */
    static const QList<WhereMemFn> cached {
        bind(&PostgresGrammar::whereBasic),
        bind(&PostgresGrammar::whereNested),
        bind(&PostgresGrammar::whereColumn),
        bind(&PostgresGrammar::whereIn),
        bind(&PostgresGrammar::whereNotIn),
        bind(&PostgresGrammar::whereNull),
        bind(&PostgresGrammar::whereNotNull),
        bind(&PostgresGrammar::whereRaw),
        bind(&PostgresGrammar::whereExists),
        bind(&PostgresGrammar::whereNotExists),
        bind(&PostgresGrammar::whereRowValues),
        bind(&PostgresGrammar::whereBetween),
        bind(&PostgresGrammar::whereBetweenColumns),
        bind(&PostgresGrammar::whereDate),
        bind(&PostgresGrammar::whereTime),
        bind(&PostgresGrammar::whereDay),
        bind(&PostgresGrammar::whereMonth),
        bind(&PostgresGrammar::whereYear),
    };

    static const auto size = cached.size();

    // Check if whereType is in the range, just for sure 😏
    using SizeType = std::remove_const_t<decltype (cached)>::size_type;
    const auto type = static_cast<SizeType>(whereType);
    Q_ASSERT((type >= 0) && (type < size));

    return cached.at(type);
}

QString PostgresGrammar::compileColumns(const QueryBuilder &query) const
{
    QString select;

    const auto &distinct = query.getDistinct();

    if (std::holds_alternative<QStringList>(distinct))
        select += u"select distinct on (%1) "_s
                  .arg(columnize(std::get<QStringList>(distinct)));

    else if (std::holds_alternative<bool>(distinct) && std::get<bool>(distinct))
        select += u"select distinct "_s;

    else
        select += u"select "_s;

    return select += columnize(query.getColumns());
}

QString PostgresGrammar::whereDate(const WhereConditionItem &where) const
{
    return u"%1::date %3 %4"_s.arg(wrap(where.column),
                                   where.comparison,
                                   parameter(where.value));
}

QString PostgresGrammar::whereTime(const WhereConditionItem &where) const
{
    return u"%1::time %3 %4"_s.arg(wrap(where.column),
                                   where.comparison,
                                   parameter(where.value));
}

QString PostgresGrammar::dateBasedWhere(const QString &type,
                                        const WhereConditionItem &where) const
{
    return u"extract(%1 from %2) %3 %4"_s.arg(type,
                                              wrap(where.column),
                                              where.comparison,
                                              parameter(where.value));
}

QString PostgresGrammar::compileUpdateColumns(const QList<UpdateItem> &values) const
{
    QStringList compiledAssignments;
    compiledAssignments.reserve(values.size());

    for (const auto &assignment : values)
        compiledAssignments << u"%1 = %2"_s.arg(wrap(unqualifyColumn(assignment.column)),
                                                parameter(assignment.value));

    return columnizeWithoutWrap(compiledAssignments);
}

/* private */

QString PostgresGrammar::compileUpdateWithJoinsOrLimit(
        QueryBuilder &query, const QList<UpdateItem> &values) const
{
    const auto table = std::get<QString>(query.getFrom());

    const auto tableWrapped = wrapTable(table);

    const auto columns = compileUpdateColumns(values);

    const auto alias = getAliasFromFrom(table);

    const auto selectSql = compileSelect(query.select(u"%1.ctid"_s.arg(alias)));

    return u"update %1 set %2 where %3 in (%4)"_s
            .arg(tableWrapped, columns, wrap(u"ctid"_s), selectSql);
}

QString PostgresGrammar::compileDeleteWithJoinsOrLimit(QueryBuilder &query) const
{
    const auto table = std::get<QString>(query.getFrom());

    const auto tableWrapped = wrapTable(table);

    const auto alias = getAliasFromFrom(table);

    const auto selectSql = compileSelect(query.select(u"%1.ctid"_s.arg(alias)));

    return u"delete from %1 where %2 in (%3)"_s
            .arg(tableWrapped, wrap(u"ctid"_s), selectSql);
}

} // namespace Orm::Query::Grammars

TINYORM_END_COMMON_NAMESPACE
