#include "aobjectsystem.h"

#include "alazyobject.h"
#include "auri.h"

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

AObject *AObjectSystem::createObject(const string &url, AObject *parent) {
    AObject *pObject        = 0;
    factoryMap::iterator it = mFactories.find(url);
    if(it == mFactories.end()) {
        it  = mFactories.find(mGroups[url]);
    }
    if(it != mFactories.end()) {
        pObject = new ALazyObject((*it).second);
        if(pObject) {
            pObject->setParent(parent);
        }
    }

    return pObject;
}

void AObjectSystem::factoryAdd(const string &uri, AObject *prototype) {
    if(prototype) {
        AUri group(uri);
        mGroups[group.name()]   = uri;
        mFactories[uri]         = prototype;
    }
}

void AObjectSystem::factoryRemove(const string &uri) {
    mFactories.erase(uri);
}

void AObjectSystem::factoryClear() {
    mFactories.clear();
}

groupMap AObjectSystem::factory() const {
    return mGroups;
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
