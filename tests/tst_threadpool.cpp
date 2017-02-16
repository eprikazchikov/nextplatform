#include "tst_threadpool.h"

#include "athreadpool.h"

#include <QDebug>
#include <QTest>

#include "tst_common.h"

class AThreadObject : public AObject {
public:
    explicit AThreadObject     (AObject *parent = nullptr) :
            AObject(parent) {
        m_Counter   = 0;
    }

    void            post            () {
        AEvent *e   = new AEvent(AEvent::UserType);
        postEvent(e);
    }

    bool            event           (AEvent *e) {
        if(e->type() == AEvent::UserType) {
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
    qDebug() << "Optimal number of threads =" << AThreadPool::optimalThreadCount();
    m_pPool = new AThreadPool;
}

void ThreadPoolTest::cleanupTestCase() {
    delete m_pPool;
}

void ThreadPoolTest::Multi_Task() {
    AThreadObject obj;
    for(int i = 0; i < 16; i++) {
        AThreadObject *object = new AThreadObject(&obj);
        object->post();
        m_pPool->start(*object);
    }
    m_pPool->waitForDone();

    for(auto it : obj.findChildren<AThreadObject *>()) {
        QCOMPARE(it->counter(), uint32_t(1));
    }
}
