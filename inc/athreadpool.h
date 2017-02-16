#ifndef ATHREADPOOL_H
#define ATHREADPOOL_H

#include <stdint.h>
#include <thread>
#include <set>

#include "aobject.h"

class AThreadPool {
public:
    AThreadPool                 ();

    ~AThreadPool                ();

    void                        start                       (AObject &object);

    uint32_t                    maxThreads                  () const;

    void                        setMaxThreads               (uint32_t value);

    bool                        waitForDone                 (int32_t msecs = -1);

    static uint32_t             optimalThreadCount          ();

protected:
    class APoolWorker {
    public:
        APoolWorker             (AThreadPool *pool);

        ~APoolWorker            ();

        void                    exec                        ();

        void                    run                         (AObject *object);

        bool                    isFree                      ();

    protected:
        bool                    m_Enabled;

        thread                  m_Thread;

        condition_variable      m_Variable;

        AObject                *m_pTask;

        AThreadPool            *m_pPool;
    };

private:
    friend class ThreadPoolTest;

    AObject                    *takeTask                    ();

    condition_variable          m_Variable;

    mutex                       m_Mutex;

    set<APoolWorker *>          m_Workers;

    queue<AObject *>            m_Tasks;

    int32_t                     m_ActiveThreads;
};

#endif // ATHREADPOOL_H
