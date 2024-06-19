#include <QCoreApplication>
#include <QtTest>

#include "orm/drivers/exceptions/logicerror.hpp"
#include "orm/drivers/sqlquery.hpp"
#include "orm/drivers/sqlrecord.hpp"

#ifdef TINYDRIVERS_MYSQL_DRIVER
#  include "orm/drivers/mysql/version.hpp"
#endif

#include "orm/constants.hpp"
#include "orm/utils/nullvariant.hpp"
#include "orm/utils/type.hpp"

#include "databases.hpp"
#include "macros.hpp"

using namespace Qt::StringLiterals; /* NOLINT(google-build-using-namespace) */

using Orm::Constants::CREATED_AT;
using Orm::Constants::DELETED_AT;
using Orm::Constants::ID;
using Orm::Constants::NAME;
using Orm::Constants::NOTE;
using Orm::Constants::UPDATED_AT;
using Orm::Constants::dummy_NONEXISTENT;

using Orm::Drivers::Exceptions::LogicError;
using Orm::Drivers::SqlQuery;

using enum Orm::Drivers::CursorPosition;

using Orm::Utils::NullVariant;

using TypeUtils = Orm::Utils::Type;

using TestUtils::Databases;

class tst_SqlQuery_Normal : public QObject // clazy:exclude=ctor-missing-parent-argument
{
    Q_OBJECT

    /*! Expose the NumericalPrecisionPolicy enum. */
    using enum Orm::Drivers::NumericalPrecisionPolicy;

private Q_SLOTS:
    void initTestCase() const;

    void select_All() const;
    void select_EmptyResultSet() const;
    void select_FetchingSameResultSet() const;
    void select_WithWhere() const;

    void select_IsNull() const;

    void select_Aggregate_Count() const;

    void select_Testing_recordCached() const;

    void seeking() const;

    void finish_And_detachFromResultSet() const;

    void select_reExecute_SameQuery() const;
    void select_executeAnotherQuery_OnSameInstance() const;

    void select_reExecute_SameQuery_AfterFinish() const;
    void select_executeAnotherQuery_OnSameInstance_AfterFinish() const;

    void insert_update_delete() const;

// NOLINTNEXTLINE(readability-redundant-access-specifiers)
private:
    /*! Create QueryBuilder instance for the given connection. */
    [[nodiscard]] inline static SqlQuery createQuery(const QString &connection);
};

/* private slots */

// NOLINTBEGIN(readability-convert-member-functions-to-static)
void tst_SqlQuery_Normal::initTestCase() const
{
    const auto connections = Databases::createDriversConnections();

    if (connections.isEmpty())
        QSKIP(TestUtils::AutoTestSkippedAny.arg(TypeUtils::classPureBasename(*this))
                                           .toUtf8().constData(), );

    QTest::addColumn<QString>("connection");

    // Run all tests for all supported database connections
    for (const auto &connection : connections)
        QTest::newRow(connection.toUtf8().constData()) << connection;
}

/*! Comparable structure to store the ID and custom templated type. */
template<typename T>
struct IdAndCustomType
{
    /*! Record's ID column. */
    quint64 id;
    /*! Any column. */
    T custom;

    /*! Equality comparison operator for the IdAndCustomType. */
    bool operator==(const IdAndCustomType &) const = default;
};

void tst_SqlQuery_Normal::select_All() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto users = createQuery(connection);

    const auto query = u"select id, name from users order by id"_s;
    const auto ok = users.exec(query);

    // Check everything what can be checked for this basic query (default configuration)
    QVERIFY(ok);
    QVERIFY(users.isActive());
    QVERIFY(users.isSelect());
    QVERIFY(!users.isValid());
    QCOMPARE(users.at(), BeforeFirstRow);
    const auto querySize = users.size();
    QCOMPARE(querySize, 5);
    // Behaves the same as the size() for SELECT queries
    QCOMPARE(users.numRowsAffected(), 5);
    QCOMPARE(users.numericalPrecisionPolicy(), LowPrecisionDouble);
    QCOMPARE(users.executedQuery(), query);
    QCOMPARE(users.lastInsertId(), QVariant());

    QVERIFY(users.boundValues().isEmpty());

    // Verify the result
    QVector<IdAndCustomType<QString>> expected {
        {1, "andrej"}, {2, "silver"}, {3, "peter"}, {4, "jack"}, {5, "obiwan"},
    };
    QVector<IdAndCustomType<QString>> actual;
    actual.reserve(querySize);

    while (users.next()) {
        QVERIFY(users.isValid());
        QVERIFY(!users.isNull(0));
        QVERIFY(!users.isNull(1));
        QVERIFY(!users.isNull(ID));
        QVERIFY(!users.isNull(NAME));
        // Number of fields
        const auto &record = users.recordCached();
        QCOMPARE(record.count(), 2);
        QVERIFY(record.contains(ID));
        QVERIFY(record.contains(NAME));

        actual.emplaceBack(users.value(ID).value<quint64>(),
                           users.value(NAME).value<QString>());
    }
    QCOMPARE(actual, expected);
    QCOMPARE(users.at(), AfterLastRow);
}

void tst_SqlQuery_Normal::select_EmptyResultSet() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto users = createQuery(connection);

    const auto query = u"select id, name from users where name = '%1'"_s
                       .arg(dummy_NONEXISTENT);
    const auto ok = users.exec(query);

    QVERIFY(ok);
    QVERIFY(users.isActive());
    QVERIFY(users.isSelect());
    QVERIFY(!users.isValid());
    QCOMPARE(users.at(), BeforeFirstRow);
    const auto querySize = users.size();
    QCOMPARE(querySize, 0);
    // Behaves the same as the size() for SELECT queries
    QCOMPARE(users.numRowsAffected(), 0);
    QCOMPARE(users.executedQuery(), query);
    QCOMPARE(users.lastInsertId(), QVariant());

    QVERIFY(users.boundValues().isEmpty());

    QVERIFY(!users.next());
    QCOMPARE(users.at(), BeforeFirstRow);
    QVERIFY(!users.previous());
    QCOMPARE(users.at(), BeforeFirstRow);
    QVERIFY(!users.first());
    QCOMPARE(users.at(), BeforeFirstRow);
    QVERIFY(!users.last());
    QCOMPARE(users.at(), BeforeFirstRow);
    QVERIFY(!users.seek(1));
    QCOMPARE(users.at(), BeforeFirstRow);
}

void tst_SqlQuery_Normal::select_FetchingSameResultSet() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto users = createQuery(connection);

    const auto query = u"select id, name from users where id <= 3"_s;
    const auto ok = users.exec(query);

    QVERIFY(ok);
    QVERIFY(users.isActive());
    QVERIFY(users.isSelect());
    QVERIFY(!users.isValid());
    QCOMPARE(users.at(), BeforeFirstRow);
    const auto querySize = users.size();
    QCOMPARE(querySize, 3);
    // Behaves the same as the size() for SELECT queries
    QCOMPARE(users.numRowsAffected(), 3);
    QCOMPARE(users.executedQuery(), query);
    QCOMPARE(users.lastInsertId(), QVariant());

    QVERIFY(users.boundValues().isEmpty());

    QVERIFY(users.first());
    QVERIFY(users.first());

    QVERIFY(users.last());
    QVERIFY(users.last());

    QVERIFY(users.seek(1));
    QVERIFY(users.seek(1));
}

void tst_SqlQuery_Normal::select_WithWhere() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto users = createQuery(connection);

    const auto query = u"select id, name from users where id < 4 order by id"_s;
    const auto ok = users.exec(query);

    QVERIFY(ok);
    QVERIFY(users.isActive());
    QVERIFY(users.isSelect());
    QVERIFY(!users.isValid());
    QCOMPARE(users.at(), BeforeFirstRow);
    const auto querySize = users.size();
    QCOMPARE(querySize, 3);
    // Behaves the same as the size() for SELECT queries
    QCOMPARE(users.numRowsAffected(), 3);
    QCOMPARE(users.executedQuery(), query);

    QVERIFY(users.boundValues().isEmpty());

    // Verify the result
    QVector<IdAndCustomType<QString>> expected {
        {1, u"andrej"_s}, {2, u"silver"_s}, {3, u"peter"_s},
    };
    QVector<IdAndCustomType<QString>> actual;
    actual.reserve(querySize);

    while (users.next()) {
        QVERIFY(users.isValid());
        QVERIFY(!users.isNull(0));
        QVERIFY(!users.isNull(1));
        QVERIFY(!users.isNull(ID));
        QVERIFY(!users.isNull(NAME));
        // Number of fields
        const auto &record = users.recordCached();
        QCOMPARE(record.count(), 2);
        QVERIFY(record.contains(ID));
        QVERIFY(record.contains(NAME));

        actual.emplaceBack(users.value(ID).value<quint64>(),
                           users.value(NAME).value<QString>());
    }
    QCOMPARE(actual, expected);
    QCOMPARE(users.at(), AfterLastRow);
}

void tst_SqlQuery_Normal::select_IsNull() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto users = createQuery(connection);

    const auto query = u"select id, note from users order by id"_s;
    const auto ok = users.exec(query);

    QVERIFY(ok);
    QVERIFY(users.isActive());
    QVERIFY(users.isSelect());
    QVERIFY(!users.isValid());
    QCOMPARE(users.at(), BeforeFirstRow);
    const auto querySize = users.size();
    QCOMPARE(querySize, 5);
    QCOMPARE(users.executedQuery(), query);

    // Verify the result
    QVector<IdAndCustomType<bool>> expected {
        {1, true}, {2, true}, {3, false}, {4, false}, {5, false},
    };
    QVector<IdAndCustomType<bool>> actual;
    actual.reserve(querySize);

    while (users.next()) {
        QVERIFY(users.isValid());
        // Number of fields
        const auto &record = users.recordCached();
        QCOMPARE(record.count(), 2);
        QVERIFY(record.contains(ID));
        QVERIFY(record.contains(NOTE));

        actual.emplaceBack(users.value(ID).value<quint64>(),
                           users.isNull(NOTE));
    }
    QCOMPARE(actual, expected);
    QCOMPARE(users.at(), AfterLastRow);
}

void tst_SqlQuery_Normal::select_Aggregate_Count() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto users = createQuery(connection);

    const auto query = u"select count(id) as aggregate from users where id < 3"_s;
    auto ok = users.exec(query);

    QVERIFY(ok);
    QVERIFY(users.isActive());
    QVERIFY(users.isSelect());
    QVERIFY(!users.isValid());
    QCOMPARE(users.at(), BeforeFirstRow);
    const auto querySize = users.size();
    QCOMPARE(querySize, 1);
    QCOMPARE(users.executedQuery(), query);

    // Verify the result
    ok = users.first();
    QVERIFY(ok);
    QVERIFY(users.isValid());
    // Number of fields
    const auto &record = users.recordCached();
    QCOMPARE(record.count(), 1);
    QVERIFY(record.contains("aggregate"));

    QCOMPARE(users.value("aggregate").value<quint64>(), 2);
}

void tst_SqlQuery_Normal::select_Testing_recordCached() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto users = createQuery(connection);

    const auto query = u"select id, name, is_banned, note from users order by id"_s;
    auto ok = users.exec(query);

    QVERIFY(ok);
    QVERIFY(users.isActive());
    QVERIFY(users.isSelect());
    QVERIFY(!users.isValid());
    QCOMPARE(users.at(), BeforeFirstRow);
    const auto querySize = users.size();
    QCOMPARE(querySize, 5);
    // Behaves the same as the size() for SELECT queries
    QCOMPARE(users.numRowsAffected(), 5);
    QCOMPARE(users.numericalPrecisionPolicy(), LowPrecisionDouble);
    QCOMPARE(users.executedQuery(), query);
    QCOMPARE(users.lastInsertId(), QVariant());

    // Verify the result
    QVector<QVector<QVariant>> expected {
        {1, u"andrej"_s, 0, NullVariant::QString()},
        {2, u"silver"_s, 0, NullVariant::QString()},
        {3, u"peter"_s, 1, u"no torrents no roles"_s},
        {4, u"jack"_s, 0, u"test SoftDeletes"_s},
        {5, u"obiwan"_s, 1, u"test SoftDeletes"_s},
    };
    QVector<QVector<QVariant>> actual;
    actual.reserve(querySize);

    QVector<bool> expectedNull {true, true, false, false, false};
    QVector<bool> actualNull;
    actualNull.reserve(querySize);

    while (users.next()) {
        QVERIFY(users.isValid());
        actualNull << users.isNull(NOTE);
        // Number of fields
        const auto &record = users.recordCached();
        QCOMPARE(record.count(), 4);
        QVERIFY(record.contains(ID));
        QVERIFY(record.contains(NAME));

        actual << QVector<QVariant>({users.value(ID).value<quint64>(),
                                     users.value(NAME).value<QString>(),
                                     users.value("is_banned").value<QString>(),
                                     users.value(NOTE).value<QString>()});
    }
    QCOMPARE(actual, expected);
    QCOMPARE(actualNull, expectedNull);
    QCOMPARE(users.at(), AfterLastRow);
}

void tst_SqlQuery_Normal::seeking() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto users = createQuery(connection);

    const auto query = u"select id from users order by id"_s;
    const auto ok = users.exec(query);

    QVERIFY(ok);
    QVERIFY(users.isActive());
    QVERIFY(users.isSelect());
    QVERIFY(!users.isValid());
    const auto querySize = users.size();
    QCOMPARE(querySize, 5);

    // Verify the result
    QVector<quint64> expected {1, 2, 3, 4, 5};
    QVector<quint64> actual;
    actual.reserve(querySize);

    // First verify if we have the correct result set
    while (users.next()) {
        QVERIFY(!users.isNull(ID));
        // Number of fields
        const auto &record = users.recordCached();
        QCOMPARE(record.count(), 1);
        QVERIFY(record.contains(ID));

        actual.emplaceBack(users.value(ID).value<quint64>());
    }
    QCOMPARE(actual, expected);

    QCOMPARE(users.at(), AfterLastRow);

    // The following tests test all possible seeking code branches

    /* Testing first(), last(), next(), previous() */
    QVERIFY(users.first());
    QCOMPARE(users.at(), 0);
    QCOMPARE(users.value<quint64>(ID), 1);

    QVERIFY(!users.previous());
    QCOMPARE(users.at(), BeforeFirstRow);
    QVERIFY(!users.previous());
    QCOMPARE(users.at(), BeforeFirstRow);

    QVERIFY(users.next());
    QCOMPARE(users.at(), 0);
    QCOMPARE(users.value<quint64>(ID), 1);

    QVERIFY(users.last());
    QCOMPARE(users.at(), 4);
    QCOMPARE(users.value<quint64>(ID), 5);

    QVERIFY(!users.next());
    QCOMPARE(users.at(), AfterLastRow);
    QVERIFY(!users.next());
    QCOMPARE(users.at(), AfterLastRow);

    QVERIFY(users.previous());
    QCOMPARE(users.at(), 4);
    QCOMPARE(users.value<quint64>(ID), 5);

    QVERIFY(users.previous());
    QCOMPARE(users.at(), 3);
    QCOMPARE(users.value<quint64>(ID), 4);

    /* Testing seek() - seekArbitrary() */
    // In mapSeekToFetch()
    QVERIFY(!users.seek(std::numeric_limits<int>::max(), false));
    QCOMPARE(users.at(), AfterLastRow);
    // In seekArbitrary()
    QVERIFY(!users.seek(-3, false));
    QCOMPARE(users.at(), BeforeFirstRow);

    // Branch - fetch()
    QVERIFY(users.seek(4, false));
    QCOMPARE(users.at(), 4);
    QCOMPARE(users.value<quint64>(ID), 5);

    // Branch - fetchPrevious()
    QVERIFY(users.seek(3, false));
    QCOMPARE(users.at(), 3);
    QCOMPARE(users.value<quint64>(ID), 4);

    // The BeforeFirstRow branch for fetchPrevious() is unreachable with non-relative seek

    // Branch - fetchNext()
    QVERIFY(users.seek(4, false));
    QCOMPARE(users.at(), 4);
    QCOMPARE(users.value<quint64>(ID), 5);

    QVERIFY(!users.seek(5, false));
    QCOMPARE(users.at(), AfterLastRow);

    /* From here things start to be a little messy because relative seek() has
       a lot of branches. */

    /* Testing relative seek() - seekRelative() */
    // AfterLastRow
    QCOMPARE(users.at(), AfterLastRow);
    QVERIFY(!users.seek(0, true));
    QCOMPARE(users.at(), AfterLastRow);
    QVERIFY(!users.seek(1, true));
    QCOMPARE(users.at(), AfterLastRow);

    // BeforeFirstRow
    // Prepare
    QVERIFY(users.first());
    QCOMPARE(users.at(), 0);
    QVERIFY(!users.previous());
    QCOMPARE(users.at(), BeforeFirstRow);
    // Test
    QVERIFY(!users.seek(0, true));
    QCOMPARE(users.at(), BeforeFirstRow);
    QVERIFY(!users.seek(-1, true));
    QCOMPARE(users.at(), BeforeFirstRow);

    // default: BeforeFirstRow
    // Prepare
    QVERIFY(users.seek(2, false));
    QCOMPARE(users.at(), 2);
    // Test
    QVERIFY(!users.seek(-3, true));
    QCOMPARE(users.at(), BeforeFirstRow);

    // Legend: -- means or distinguishes the main sub-section

    /* Testing relative seek() - fetchNext(), fetchPrevious(), and fetch() */
    // -- BeforeFirstRow in seekRelative()

    // Branches - fetchNext() and fetchPrevious() are unreachable

    // Branch - fetch()
    QVERIFY(users.seek(5, true));
    QCOMPARE(users.at(), 4);
    QCOMPARE(users.value<quint64>(ID), 5);

    // AfterLastRow in mapSeekToFetch()
    // Prepare
    QVERIFY(users.first());
    QCOMPARE(users.at(), 0);
    QVERIFY(!users.previous());
    QCOMPARE(users.at(), BeforeFirstRow);
    // Test
    QVERIFY(!users.seek(6, true));
    QCOMPARE(users.at(), AfterLastRow);

    // -- AfterLastRow in seekRelative()

    // Branches - fetchPrevious(), fetchNext() and AfterLastRow in mapSeekToFetch() are unreachable

    // Branch - fetch()
    QVERIFY(users.seek(-4, true));
    QCOMPARE(users.at(), 1);
    QCOMPARE(users.value<quint64>(ID), 2);

    // -- default: in seekRelative()
    // Branch - fetchNext()
    QVERIFY(users.seek(1, true));
    QCOMPARE(users.at(), 2);
    QCOMPARE(users.value<quint64>(ID), 3);

    // Branch - fetchPrevious()
    QVERIFY(users.seek(-1, true));
    QCOMPARE(users.at(), 1);
    QCOMPARE(users.value<quint64>(ID), 2);

    // Branch - fetch()
    QVERIFY(users.seek(2, true));
    QCOMPARE(users.at(), 3);
    QCOMPARE(users.value<quint64>(ID), 4);

    // AfterLastRow in mapSeekToFetch()
    QVERIFY(!users.seek(2, true));
    QCOMPARE(users.at(), AfterLastRow);
}

void tst_SqlQuery_Normal::finish_And_detachFromResultSet() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto users = createQuery(connection);

    const auto query = u"select id, name from users order by id"_s;
    const auto ok = users.exec(query);

    // Check everything what can be checked for this basic query (default configuration)
    QVERIFY(ok);
    QVERIFY(users.isActive());
    QVERIFY(users.isSelect());
    QVERIFY(!users.isValid());
    const auto querySize = users.size();
    QCOMPARE(querySize, 5);
    // Behaves the same as the size() for SELECT queries
    QCOMPARE(users.numRowsAffected(), 5);
    QCOMPARE(users.numericalPrecisionPolicy(), LowPrecisionDouble);
    QCOMPARE(users.executedQuery(), query);
    QCOMPARE(users.lastInsertId(), QVariant());

    QVERIFY(users.boundValues().isEmpty());

    // Verify the result
    QVector<IdAndCustomType<QString>> expected {
        {1, "andrej"}, {2, "silver"}, {3, "peter"}, {4, "jack"}, {5, "obiwan"},
    };
    QVector<IdAndCustomType<QString>> actual;
    actual.reserve(querySize);

    while (users.next()) {
        QVERIFY(users.isValid());
        QVERIFY(!users.isNull(0));
        QVERIFY(!users.isNull(1));
        QVERIFY(!users.isNull(ID));
        QVERIFY(!users.isNull(NAME));
        // Number of fields
        const auto &record = users.recordCached();
        QCOMPARE(record.count(), 2);
        QVERIFY(record.contains(ID));
        QVERIFY(record.contains(NAME));

        actual.emplaceBack(users.value(ID).value<quint64>(),
                           users.value(NAME).value<QString>());
    }
    QCOMPARE(actual, expected);

    users.finish();

    QVERIFY(!users.isActive());
    QVERIFY(users.isSelect());
    QVERIFY(!users.isValid());
    QCOMPARE(users.at(), BeforeFirstRow);
    TVERIFY_THROWS_EXCEPTION(LogicError, users.size());
    // Behaves the same as the size() for SELECT queries
    TVERIFY_THROWS_EXCEPTION(LogicError, users.numRowsAffected());
    QCOMPARE(users.numericalPrecisionPolicy(), LowPrecisionDouble);
    QCOMPARE(users.executedQuery(), query);
    TVERIFY_THROWS_EXCEPTION(LogicError, users.lastInsertId());

    QVERIFY(!users.next());
    QVERIFY(!users.previous());
    QVERIFY(!users.first());
    QVERIFY(!users.last());
    QVERIFY(!users.seek(1));
}

void tst_SqlQuery_Normal::select_reExecute_SameQuery() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto users = createQuery(connection);

    const auto query = u"select id, name from users order by id"_s;

    QVector<IdAndCustomType<QString>> expected {
        {1, "andrej"}, {2, "silver"}, {3, "peter"}, {4, "jack"}, {5, "obiwan"},
    };

    // Execute first time
    {
        const auto ok = users.exec(query);

        QVERIFY(ok);
        QVERIFY(users.isActive());
        QVERIFY(users.isSelect());
        QVERIFY(!users.isValid());
        QCOMPARE(users.at(), BeforeFirstRow);
        const auto querySize = users.size();
        QCOMPARE(querySize, 5);
        // Behaves the same as the size() for SELECT queries
        QCOMPARE(users.numRowsAffected(), 5);
        QCOMPARE(users.numericalPrecisionPolicy(), LowPrecisionDouble);
        QCOMPARE(users.executedQuery(), query);
        QCOMPARE(users.lastInsertId(), QVariant());

        QVERIFY(users.boundValues().isEmpty());

        // Verify the result
        QVector<IdAndCustomType<QString>> actual;
        actual.reserve(querySize);

        while (users.next()) {
            QVERIFY(users.isValid());
            QVERIFY(!users.isNull(0));
            QVERIFY(!users.isNull(1));
            QVERIFY(!users.isNull(ID));
            QVERIFY(!users.isNull(NAME));
            // Number of fields
            const auto &record = users.recordCached();
            QCOMPARE(record.count(), 2);
            QVERIFY(record.contains(ID));
            QVERIFY(record.contains(NAME));

            actual.emplaceBack(users.value(ID).value<quint64>(),
                               users.value(NAME).value<QString>());
        }
        QCOMPARE(actual, expected);
        QCOMPARE(users.at(), AfterLastRow);
    }

    // Re-execute second time
    {
        const auto ok = users.exec(query);

        QVERIFY(ok);
        QVERIFY(users.isActive());
        QVERIFY(users.isSelect());
        QVERIFY(!users.isValid());
        QCOMPARE(users.at(), BeforeFirstRow);
        const auto querySize = users.size();
        QCOMPARE(querySize, 5);
        // Behaves the same as the size() for SELECT queries
        QCOMPARE(users.numRowsAffected(), 5);
        QCOMPARE(users.numericalPrecisionPolicy(), LowPrecisionDouble);
        QCOMPARE(users.executedQuery(), query);
        QCOMPARE(users.lastInsertId(), QVariant());

        QVERIFY(users.boundValues().isEmpty());

        // Verify the result
        QVector<IdAndCustomType<QString>> actual;
        actual.reserve(querySize);

        while (users.next()) {
            QVERIFY(users.isValid());
            QVERIFY(!users.isNull(0));
            QVERIFY(!users.isNull(1));
            QVERIFY(!users.isNull(ID));
            QVERIFY(!users.isNull(NAME));
            // Number of fields
            const auto &record = users.recordCached();
            QCOMPARE(record.count(), 2);
            QVERIFY(record.contains(ID));
            QVERIFY(record.contains(NAME));

            actual.emplaceBack(users.value(ID).value<quint64>(),
                               users.value(NAME).value<QString>());
        }
        QCOMPARE(actual, expected);
        QCOMPARE(users.at(), AfterLastRow);
    }
}

void tst_SqlQuery_Normal::select_executeAnotherQuery_OnSameInstance() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto query = createQuery(connection);

    // Execute first time
    {
        const auto queryString = u"select id, name from users order by id"_s;
        const auto ok = query.exec(queryString);

        QVERIFY(ok);
        QVERIFY(query.isActive());
        QVERIFY(query.isSelect());
        QVERIFY(!query.isValid());
        QCOMPARE(query.at(), BeforeFirstRow);
        const auto querySize = query.size();
        QCOMPARE(querySize, 5);
        // Behaves the same as the size() for SELECT queries
        QCOMPARE(query.numRowsAffected(), 5);
        QCOMPARE(query.numericalPrecisionPolicy(), LowPrecisionDouble);
        QCOMPARE(query.executedQuery(), queryString);
        QCOMPARE(query.lastInsertId(), QVariant());

        QVERIFY(query.boundValues().isEmpty());

        // Verify the result
        QVector<IdAndCustomType<QString>> expected {
            {1, "andrej"}, {2, "silver"}, {3, "peter"}, {4, "jack"}, {5, "obiwan"},
        };
        QVector<IdAndCustomType<QString>> actual;
        actual.reserve(querySize);

        while (query.next()) {
            QVERIFY(query.isValid());
            QVERIFY(!query.isNull(0));
            QVERIFY(!query.isNull(1));
            QVERIFY(!query.isNull(ID));
            QVERIFY(!query.isNull(NAME));
            // Number of fields
            const auto &record = query.recordCached();
            QCOMPARE(record.count(), 2);
            QVERIFY(record.contains(ID));
            QVERIFY(record.contains(NAME));

            actual.emplaceBack(query.value(ID).value<quint64>(),
                               query.value(NAME).value<QString>());
        }
        QCOMPARE(actual, expected);
        QCOMPARE(query.at(), AfterLastRow);
    }

    // Re-execute second time
    {
        const auto queryString = u"select * from roles order by id"_s;
        const auto ok = query.exec(queryString);

        QVERIFY(ok);
        QVERIFY(query.isActive());
        QVERIFY(query.isSelect());
        QVERIFY(!query.isValid());
        QCOMPARE(query.at(), BeforeFirstRow);
        const auto querySize = query.size();
        QCOMPARE(querySize, 3);
        // Behaves the same as the size() for SELECT queries
        QCOMPARE(query.numRowsAffected(), 3);
        QCOMPARE(query.numericalPrecisionPolicy(), LowPrecisionDouble);
        QCOMPARE(query.executedQuery(), queryString);
        QCOMPARE(query.lastInsertId(), QVariant());

        QVERIFY(query.boundValues().isEmpty());

        // Verify the result
        QVector<IdAndCustomType<QString>> expected {
            {1, "role one"}, {2, "role two"}, {3, "role three"},
        };
        QVector<IdAndCustomType<QString>> actual;
        actual.reserve(querySize);

        while (query.next()) {
            QVERIFY(query.isValid());
            QVERIFY(!query.isNull(0));
            QVERIFY(!query.isNull(1));
            QVERIFY(!query.isNull(ID));
            QVERIFY(!query.isNull(NAME));
            // Number of fields
            const auto &record = query.recordCached();
            QCOMPARE(record.count(), 3);
            QVERIFY(record.contains(ID));
            QVERIFY(record.contains(NAME));
            QVERIFY(record.contains(u"added_on"_s));

            actual.emplaceBack(query.value(ID).value<quint64>(),
                               query.value(NAME).value<QString>());
        }
        QCOMPARE(actual, expected);
        QCOMPARE(query.at(), AfterLastRow);
    }
}

void tst_SqlQuery_Normal::select_reExecute_SameQuery_AfterFinish() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto users = createQuery(connection);

    const auto query = u"select id, name from users order by id"_s;

    QVector<IdAndCustomType<QString>> expected {
        {1, "andrej"}, {2, "silver"}, {3, "peter"}, {4, "jack"}, {5, "obiwan"},
    };

    // Execute first time
    {
        const auto ok = users.exec(query);

        QVERIFY(ok);
        QVERIFY(users.isActive());
        QVERIFY(users.isSelect());
        QVERIFY(!users.isValid());
        QCOMPARE(users.at(), BeforeFirstRow);
        const auto querySize = users.size();
        QCOMPARE(querySize, 5);
        // Behaves the same as the size() for SELECT queries
        QCOMPARE(users.numRowsAffected(), 5);
        QCOMPARE(users.numericalPrecisionPolicy(), LowPrecisionDouble);
        QCOMPARE(users.executedQuery(), query);
        QCOMPARE(users.lastInsertId(), QVariant());

        QVERIFY(users.boundValues().isEmpty());

        // Verify the result
        QVector<IdAndCustomType<QString>> actual;
        actual.reserve(querySize);

        while (users.next()) {
            QVERIFY(users.isValid());
            QVERIFY(!users.isNull(0));
            QVERIFY(!users.isNull(1));
            QVERIFY(!users.isNull(ID));
            QVERIFY(!users.isNull(NAME));
            // Number of fields
            const auto &record = users.recordCached();
            QCOMPARE(record.count(), 2);
            QVERIFY(record.contains(ID));
            QVERIFY(record.contains(NAME));

            actual.emplaceBack(users.value(ID).value<quint64>(),
                               users.value(NAME).value<QString>());
        }
        QCOMPARE(actual, expected);
        QCOMPARE(users.at(), AfterLastRow);
    }

    // Finish (detachFromResultSet())
    {
        users.finish();

        QVERIFY(!users.isActive());
        QVERIFY(users.isSelect());
        QVERIFY(!users.isValid());
        QCOMPARE(users.at(), BeforeFirstRow);
        TVERIFY_THROWS_EXCEPTION(LogicError, users.size());
        // Behaves the same as the size() for SELECT queries
        TVERIFY_THROWS_EXCEPTION(LogicError, users.numRowsAffected());
        QCOMPARE(users.numericalPrecisionPolicy(), LowPrecisionDouble);
        QCOMPARE(users.executedQuery(), query);
        TVERIFY_THROWS_EXCEPTION(LogicError, users.lastInsertId());

        QVERIFY(!users.next());
        QVERIFY(!users.previous());
        QVERIFY(!users.first());
        QVERIFY(!users.last());
        QVERIFY(!users.seek(1));
    }

    // Re-execute second time
    {
        const auto ok = users.exec(query);

        QVERIFY(ok);
        QVERIFY(users.isActive());
        QVERIFY(users.isSelect());
        QVERIFY(!users.isValid());
        QCOMPARE(users.at(), BeforeFirstRow);
        const auto querySize = users.size();
        QCOMPARE(querySize, 5);
        // Behaves the same as the size() for SELECT queries
        QCOMPARE(users.numRowsAffected(), 5);
        QCOMPARE(users.numericalPrecisionPolicy(), LowPrecisionDouble);
        QCOMPARE(users.executedQuery(), query);
        QCOMPARE(users.lastInsertId(), QVariant());

        QVERIFY(users.boundValues().isEmpty());

        // Verify the result
        QVector<IdAndCustomType<QString>> actual;
        actual.reserve(querySize);

        while (users.next()) {
            QVERIFY(users.isValid());
            QVERIFY(!users.isNull(0));
            QVERIFY(!users.isNull(1));
            QVERIFY(!users.isNull(ID));
            QVERIFY(!users.isNull(NAME));
            // Number of fields
            const auto &record = users.recordCached();
            QCOMPARE(record.count(), 2);
            QVERIFY(record.contains(ID));
            QVERIFY(record.contains(NAME));

            actual.emplaceBack(users.value(ID).value<quint64>(),
                               users.value(NAME).value<QString>());
        }
        QCOMPARE(actual, expected);
        QCOMPARE(users.at(), AfterLastRow);
    }
}

void tst_SqlQuery_Normal::select_executeAnotherQuery_OnSameInstance_AfterFinish() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto query = createQuery(connection);

    // Execute first time
    const auto queryString = u"select id, name from users order by id"_s;
    {
        const auto ok = query.exec(queryString);

        QVERIFY(ok);
        QVERIFY(query.isActive());
        QVERIFY(query.isSelect());
        QVERIFY(!query.isValid());
        QCOMPARE(query.at(), BeforeFirstRow);
        const auto querySize = query.size();
        QCOMPARE(querySize, 5);
        // Behaves the same as the size() for SELECT queries
        QCOMPARE(query.numRowsAffected(), 5);
        QCOMPARE(query.numericalPrecisionPolicy(), LowPrecisionDouble);
        QCOMPARE(query.executedQuery(), queryString);
        QCOMPARE(query.lastInsertId(), QVariant());

        QVERIFY(query.boundValues().isEmpty());

        // Verify the result
        QVector<IdAndCustomType<QString>> expected {
            {1, "andrej"}, {2, "silver"}, {3, "peter"}, {4, "jack"}, {5, "obiwan"},
        };
        QVector<IdAndCustomType<QString>> actual;
        actual.reserve(querySize);

        while (query.next()) {
            QVERIFY(query.isValid());
            QVERIFY(!query.isNull(0));
            QVERIFY(!query.isNull(1));
            QVERIFY(!query.isNull(ID));
            QVERIFY(!query.isNull(NAME));
            // Number of fields
            const auto &record = query.recordCached();
            QCOMPARE(record.count(), 2);
            QVERIFY(record.contains(ID));
            QVERIFY(record.contains(NAME));

            actual.emplaceBack(query.value(ID).value<quint64>(),
                               query.value(NAME).value<QString>());
        }
        QCOMPARE(actual, expected);
        QCOMPARE(query.at(), AfterLastRow);
    }

    // Finish (detachFromResultSet())
    {
        query.finish();

        QVERIFY(!query.isActive());
        QVERIFY(query.isSelect());
        QVERIFY(!query.isValid());
        QCOMPARE(query.at(), BeforeFirstRow);
        TVERIFY_THROWS_EXCEPTION(LogicError, query.size());
        // Behaves the same as the size() for SELECT queries
        TVERIFY_THROWS_EXCEPTION(LogicError, query.numRowsAffected());
        QCOMPARE(query.numericalPrecisionPolicy(), LowPrecisionDouble);
        QCOMPARE(query.executedQuery(), queryString);
        TVERIFY_THROWS_EXCEPTION(LogicError, query.lastInsertId());

        QVERIFY(!query.next());
        QVERIFY(!query.previous());
        QVERIFY(!query.first());
        QVERIFY(!query.last());
        QVERIFY(!query.seek(1));
    }

    // Re-execute second time
    {
        const auto queryString2 = u"select * from roles order by id"_s;
        const auto ok = query.exec(queryString2);

        QVERIFY(ok);
        QVERIFY(query.isActive());
        QVERIFY(query.isSelect());
        QVERIFY(!query.isValid());
        QCOMPARE(query.at(), BeforeFirstRow);
        const auto querySize = query.size();
        QCOMPARE(querySize, 3);
        // Behaves the same as the size() for SELECT queries
        QCOMPARE(query.numRowsAffected(), 3);
        QCOMPARE(query.numericalPrecisionPolicy(), LowPrecisionDouble);
        QCOMPARE(query.executedQuery(), queryString2);
        QCOMPARE(query.lastInsertId(), QVariant());

        QVERIFY(query.boundValues().isEmpty());

        // Verify the result
        QVector<IdAndCustomType<QString>> expected {
            {1, "role one"}, {2, "role two"}, {3, "role three"},
        };
        QVector<IdAndCustomType<QString>> actual;
        actual.reserve(querySize);

        while (query.next()) {
            QVERIFY(query.isValid());
            QVERIFY(!query.isNull(0));
            QVERIFY(!query.isNull(1));
            QVERIFY(!query.isNull(ID));
            QVERIFY(!query.isNull(NAME));
            // Number of fields
            const auto &record = query.recordCached();
            QCOMPARE(record.count(), 3);
            QVERIFY(record.contains(ID));
            QVERIFY(record.contains(NAME));
            QVERIFY(record.contains(u"added_on"_s));

            actual.emplaceBack(query.value(ID).value<quint64>(),
                               query.value(NAME).value<QString>());
        }
        QCOMPARE(actual, expected);
        QCOMPARE(query.at(), AfterLastRow);
    }
}

/* I will test the INSERT, UPDATE, and DELETE in the one test method, it's nothing
   wrong about it (it's not absolutely cosher though), it also tests executing more
   queries on the same SqlQuery instance. */
void tst_SqlQuery_Normal::insert_update_delete() const
{
    QFETCH_GLOBAL(QString, connection); // NOLINT(modernize-type-traits)

    auto users = createQuery(connection);
    quint64 lastInsertedId = 0;

    // INSERT a new row into the users table
    {
        const auto query =
                u"insert into users "
                   "(name, is_banned, note, created_at, updated_at, deleted_at) "
                 "values ('ashen one', 1, 'test drivers INSERT', "
                   "'2023-05-11T11:52:53', '2023-05-12T11:52:53', null)"_s;
        const auto ok = users.exec(query);

        QVERIFY(ok);
        QVERIFY(users.isActive());
        QVERIFY(!users.isSelect());
        QVERIFY(!users.isValid());
        TVERIFY_THROWS_EXCEPTION(LogicError, users.size());
        QCOMPARE(users.numRowsAffected(), 1);
        QCOMPARE(users.executedQuery(), query);

        lastInsertedId = users.lastInsertId().value<quint64>();
        QVERIFY(lastInsertedId > 5);
    }

    const auto columnNames = std::to_array({ID, NAME, u"is_banned"_s, NOTE, CREATED_AT,
                                            UPDATED_AT, DELETED_AT});

    // Verify the INSERT
    {
        const auto query = u"select * from users where id = %1"_s.arg(lastInsertedId);
        auto ok = users.exec(query);

        QVERIFY(ok);
        QVERIFY(users.isActive());
        QVERIFY(users.isSelect());
        QVERIFY(!users.isValid());
        const auto querySize = users.size();
        QCOMPARE(querySize, 1);
        QCOMPARE(users.executedQuery(), query);

        // Verify the result
        ok = users.first();
        QVERIFY(ok);
        QVERIFY(users.isValid());
        // Number of fields
        const auto &record = users.recordCached();
        QCOMPARE(record.count(), 7);
        for (const auto &column : columnNames)
            QVERIFY(record.contains(column));

        // Tests if the QVariant has the correct type will be done in other test methods
        QCOMPARE(users.value(NAME)       .value<QString>(),   u"ashen one"_s);
        QCOMPARE(users.value("is_banned").value<bool>(),      true);
        QCOMPARE(users.value(NOTE)       .value<QString>(),   u"test drivers INSERT"_s);
#if tinymysql_lib_utc_qdatetime >= 20240618
        QCOMPARE(users.value(CREATED_AT) .value<QDateTime>(), QDateTime({2023, 05, 11},
                                                                        {11, 52, 53},
                                                                        QTimeZone::UTC));
        QCOMPARE(users.value(UPDATED_AT) .value<QDateTime>(), QDateTime({2023, 05, 12},
                                                                        {11, 52, 53},
                                                                        QTimeZone::UTC));
#else
        QCOMPARE(users.value(CREATED_AT) .value<QDateTime>(), QDateTime({2023, 05, 11},
                                                                        {11, 52, 53}));
        QCOMPARE(users.value(UPDATED_AT) .value<QDateTime>(), QDateTime({2023, 05, 12},
                                                                        {11, 52, 53}));
#endif
        QVERIFY(users.isNull(DELETED_AT));
        QCOMPARE(users.value(DELETED_AT), NullVariant::QDateTime());
    }

    // UPDATE
    {
        const auto query = u"update users set name = '%1', is_banned = %2 "
                              "where id = %3"_s
                           .arg("micah").arg(0).arg(lastInsertedId);
        auto ok = users.exec(query);

        QVERIFY(ok);
        QVERIFY(users.isActive());
        QVERIFY(!users.isSelect());
        QVERIFY(!users.isValid());
        TVERIFY_THROWS_EXCEPTION(LogicError, users.size());
        QCOMPARE(users.numRowsAffected(), 1);
        QCOMPARE(users.executedQuery(), query);
    }

    // Verify the UPDATE
    {
        const auto query = u"select * from users where id = %1"_s.arg(lastInsertedId);
        auto ok = users.exec(query);

        QVERIFY(ok);
        QVERIFY(users.isActive());
        QVERIFY(users.isSelect());
        QVERIFY(!users.isValid());
        const auto querySize = users.size();
        QCOMPARE(querySize, 1);
        QCOMPARE(users.executedQuery(), query);

        // Verify the result
        ok = users.first();
        QVERIFY(ok);
        QVERIFY(users.isValid());
        // Number of fields
        const auto &record = users.recordCached();
        QCOMPARE(record.count(), 7);
        for (const auto &column : columnNames)
            QVERIFY(record.contains(column));

        // Tests if the QVariant has the correct type will be done in other test methods
        QCOMPARE(users.value(NAME)       .value<QString>(),   u"micah"_s);
        QCOMPARE(users.value("is_banned").value<bool>(),      false);
        QCOMPARE(users.value(NOTE)       .value<QString>(),   u"test drivers INSERT"_s);
#if tinymysql_lib_utc_qdatetime >= 20240618
        QCOMPARE(users.value(CREATED_AT) .value<QDateTime>(), QDateTime({2023, 05, 11},
                                                                        {11, 52, 53},
                                                                        QTimeZone::UTC));
        QCOMPARE(users.value(UPDATED_AT) .value<QDateTime>(), QDateTime({2023, 05, 12},
                                                                        {11, 52, 53},
                                                                        QTimeZone::UTC));
#else
        QCOMPARE(users.value(CREATED_AT) .value<QDateTime>(), QDateTime({2023, 05, 11},
                                                                        {11, 52, 53}));
        QCOMPARE(users.value(UPDATED_AT) .value<QDateTime>(), QDateTime({2023, 05, 12},
                                                                        {11, 52, 53}));
#endif
        QVERIFY(users.isNull(DELETED_AT));
        QCOMPARE(users.value(DELETED_AT), NullVariant::QDateTime());
    }

    // Restore and also test the DELETE
    {
        const auto query = u"delete from users where id = %1"_s.arg(lastInsertedId);
        auto ok = users.exec(query);

        QVERIFY(ok);
        QVERIFY(users.isActive());
        QVERIFY(!users.isSelect());
        QVERIFY(!users.isValid());
        TVERIFY_THROWS_EXCEPTION(LogicError, users.size());
        QCOMPARE(users.numRowsAffected(), 1);
        QCOMPARE(users.executedQuery(), query);
    }

    // Verify the DELETE
    {
        const auto query = u"select id from users where id = %1"_s.arg(lastInsertedId);
        auto ok = users.exec(query);

        QVERIFY(ok);
        QVERIFY(users.isActive());
        QVERIFY(users.isSelect());
        QVERIFY(!users.isValid());
        const auto querySize = users.size();
        QCOMPARE(querySize, 0);
        QCOMPARE(users.executedQuery(), query);

        // Verify the result
        ok = users.first();
        QVERIFY(!ok);
        // Check also this, must stay the same
        QVERIFY(users.isActive());
        QVERIFY(users.isSelect());
        QVERIFY(!users.isValid());
    }
}
// NOLINTEND(readability-convert-member-functions-to-static)

/* private */

SqlQuery tst_SqlQuery_Normal::createQuery(const QString &connection)
{
   return SqlQuery(Databases::driversConnection(connection));
}

QTEST_MAIN(tst_SqlQuery_Normal)

#include "tst_sqlquery_normal.moc"
