#include "athreadpool.h"

AThreadPool::APoolWorker::APoolWorker(AThreadPool *pool) :
        m_pPool(pool),
        m_pTask(nullptr),
        m_Enabled(true) {
    PROFILE_FUNCTION()
    m_Thread    = thread(&APoolWorker::exec, this);
}

AThreadPool::APoolWorker::~APoolWorker() {
    PROFILE_FUNCTION()
    m_Enabled   = false;
    m_Variable.notify_one();
    m_Thread.join();
}

void AThreadPool::APoolWorker::exec() {
    PROFILE_FUNCTION()
    while(m_Enabled) {
        unique_lock<mutex> locker(m_pPool->m_Mutex);
        m_Variable.wait(locker, [&]() { return (m_pTask != nullptr) || !m_Enabled; });

        if(m_pTask) {
            locker.unlock();
            m_pTask->processEvents();
            locker.lock();
        }
        m_pTask = m_pPool->takeTask();
        m_Variable.notify_one();
    }
}

void AThreadPool::APoolWorker::run(AObject *object) {
    PROFILE_FUNCTION()
    ++m_pPool->m_ActiveThreads;
    m_pTask = object;
    m_Variable.notify_one();
}

bool AThreadPool::APoolWorker::isFree() {
    PROFILE_FUNCTION()
    return (m_pTask == nullptr);
}

AThreadPool::AThreadPool() :
        m_ActiveThreads(0) {
    PROFILE_FUNCTION()
    setMaxThreads(optimalThreadCount());
}

AThreadPool::~AThreadPool() {
    PROFILE_FUNCTION()
    for(auto it : m_Workers) {
        delete it;
    }
    m_Workers.clear();
}

void AThreadPool::start(AObject &object) {
    PROFILE_FUNCTION()
    unique_lock<mutex> locker(m_Mutex);
    for(auto it : m_Workers) {
        if(it->isFree()) {
            it->run(&object);
            return;
        }
    }
    m_Tasks.push(&object);
}

uint32_t AThreadPool::maxThreads() const {
    PROFILE_FUNCTION()
    return m_Workers.size();
}

void AThreadPool::setMaxThreads(uint32_t value) {
    PROFILE_FUNCTION()
    uint32_t current    = m_Workers.size();
    if(current < value) {
        for(int i = 0; i < value - current; i++) {
            APoolWorker *worker = new APoolWorker(this);
            AObject *object     = takeTask();
            if(object) {
                worker->run(object);
            }
            m_Workers.insert(worker);
        }
    } else if(current > value) {
        for(int i = 0; i < current - value; i++) {
            auto it = m_Workers.end();
            --it;
            APoolWorker *worker = (*it);
            m_Workers.erase(it);
            delete worker;
        }
    }
}

bool AThreadPool::waitForDone(int32_t msecs) {
    PROFILE_FUNCTION()
    unique_lock<mutex> locker(m_Mutex);
    if(msecs < 0) {
        while(true) {
            m_Variable.wait(locker, [&]() { return (m_Tasks.empty() && m_ActiveThreads == 0); });
            break;
        }
    }
    return (m_Tasks.empty() && m_ActiveThreads == 0);
}

uint32_t AThreadPool::optimalThreadCount() {
    PROFILE_FUNCTION()
    return thread::hardware_concurrency();
}

AObject *AThreadPool::takeTask() {
    PROFILE_FUNCTION()
    if(!m_Tasks.empty()) {
        AObject *object = m_Tasks.front();
        m_Tasks.pop();
        return object;
    }
    if(m_ActiveThreads > 0) {
        --m_ActiveThreads;
        m_Variable.notify_one();
    }
    return nullptr;
}
