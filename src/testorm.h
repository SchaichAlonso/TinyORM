#ifndef TESTORM_H
#define TESTORM_H

#include "orm/entitymanager.h"

class TestOrm
{
public:
    TestOrm() = default;

//    using KeyType = quint64;

    void run();

private:
    void anotherTests();
    void testTinyOrm();
    void testQueryBuilder();

    Orm::EntityManager m_em;
};

//class MyClass : public QObject
//{
//    Q_OBJECT
////    Q_PROPERTY(Priority priority READ priority WRITE setPriority)
//    Q_PROPERTY(Priority priority READ priority WRITE setPriority NOTIFY priorityChanged)

//public:
//    MyClass(QObject *parent = nullptr) {};
//    ~MyClass() = default;

//    enum Priority { High, Low, VeryHigh, VeryLow };
//    Q_ENUM(Priority)

//    void setPriority(Priority priority)
//    {
//        m_priority = priority;
//        emit priorityChanged(priority);
//    }
//    Priority priority() const
//    { return m_priority; }

//signals:
//    void priorityChanged(Priority);

//private:
//    Priority m_priority;
//};

#endif // TESTORM_H
