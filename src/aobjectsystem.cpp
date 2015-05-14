#include "aobjectsystem.h"

#include "aobject.h"

AObjectSystem *AObjectSystem::m_pInstance    = 0;

AObjectSystem::AObjectSystem() {
    mNextId     = 0;
    mSystemName = "system";
}

AObjectSystem::~AObjectSystem() {
    factoryClear();
}

AObjectSystem *AObjectSystem::instance() {
    if(!m_pInstance) {
        m_pInstance = new AObjectSystem;
        AObject::registerClassFactory();
    }
    return m_pInstance;
}

void AObjectSystem::destroy() {
    if(m_pInstance) {
        delete m_pInstance;
    }
    m_pInstance = 0;
}

AObject *AObjectSystem::createObject(const string &uri, AObject *parent) {
    AObject *pObject  = 0;
    factory_map::iterator it    = mFactories.find(uri);
    if(it != mFactories.end()) {
        pObject = ((*it).second)();
        if(pObject) {
            pObject->setParent(parent);
        }
    }

    return pObject;
}

void AObjectSystem::factoryAdd(const string &uri, handler_callback callback) {
    mFactories[uri] = callback;
}

void AObjectSystem::factoryRemove(const string &uri) {
    mFactories.erase(uri);
}

void AObjectSystem::factoryClear() {
    mFactories.clear();
}

bool AObjectSystem::factoryFirst(string &uri) {
    fIterator   = mFactories.begin();
    if(fIterator != mFactories.end()) {
        uri     = (*fIterator).first;
        return true;
    }
    return false;
}

bool AObjectSystem::factoryNext(string &uri) {
    fIterator++;
    if(fIterator != mFactories.end()) {
        uri     = (*fIterator).first;
        return true;
    }
    return false;
}

unsigned int AObjectSystem::nextId() {
    return ++mNextId;
}

const string &AObjectSystem::systemName() const {
    return mSystemName;
}

void AObjectSystem::setSytemName(const string &name) {
    mSystemName = name;
}
