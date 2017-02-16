#ifndef TST_THREADPOOL_H
#define TST_THREADPOOL_H

#include <QObject>

class AThreadPool;

class ThreadPoolTest : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void Multi_Task();

private:
    AThreadPool *m_pPool;

};

#endif // TST_THREADPOOL_H
