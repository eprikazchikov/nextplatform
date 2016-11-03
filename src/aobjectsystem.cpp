#include "aobjectsystem.h"

#include "aobject.h"
#include "auri.h"

AObjectSystem::AObjectSystem() {
    mSystemName = "system";
}

AObjectSystem::~AObjectSystem() {
    factoryClear();
}

AObject *AObjectSystem::objectCreate(const string &uri, AObject *parent) {
    AObject *pObject  = 0;
    factoryMap::iterator it = mFactories.find(uri);
    if(it == mFactories.end()) {
        it  = mFactories.find(mGroups[uri]);
    }
    if(it != mFactories.end()) {
        pObject = (*it).second->createInstance();
        if(pObject) {
            pObject->setParent(parent);
        }
    }

    return pObject;
}

void AObjectSystem::factoryAdd(const string &uri, AObject *prototype) {
    AUri group(uri);
    prototype->setSystem(this);
    mGroups[group.name()]   = uri;
    mFactories[uri]         = prototype;
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

const string &AObjectSystem::systemName() const {
    return mSystemName;
}

void AObjectSystem::setSytemName(const string &name) {
    mSystemName = name;
}
