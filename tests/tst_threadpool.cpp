#include "tst_threadpool.h"

#include "threadpool.h"

#include <QDebug>
#include <QTest>

#include "tst_common.h"

class ThreadObject : public Object {
public:
    explicit ThreadObject     () :
            Object() {
        m_Counter   = 0;
    }

    void            post            () {
        Event *e    = new Event(Event::USERTYPE);
        postEvent(e);
    }

    bool            event           (Event *e) {
        if(e->type() == Event::USERTYPE) {
            QTest::qSleep(1000);
            m_Counter++;
            return true;
        }
        return false;
    }

    uint32_t        counter         () const {
        return m_Counter;
    }

    uint32_t        m_Counter;
};

void ThreadPoolTest::initTestCase() {
    qDebug() << "Optimal number of threads =" << ThreadPool::optimalThreadCount();
    m_pPool = new ThreadPool;
}

void ThreadPoolTest::cleanupTestCase() {
    delete m_pPool;
}

void ThreadPoolTest::Multi_Task() {
    ThreadObject obj;
    obj.setName("MainObject");
    for(int i = 0; i < 16; i++) {
        ThreadObject *object = new ThreadObject();
        object->setName(string("TestComponent") + to_string(i));
        object->setParent(&obj);
        object->post();
        m_pPool->start(*object);
    }
    m_pPool->waitForDone();

    for(auto it : obj.findChildren<ThreadObject *>()) {
        QCOMPARE(it->counter(), uint32_t(1));
    }
}
