#include "aobjectsystem.h"

#include "aobject.h"
#include "ajson.h"

AObjectSystem *AObjectSystem::m_pInstance    = 0;

AObjectSystem::AObjectSystem() {
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

AObject *AObjectSystem::load(const string &data) {
    return 0;
}

string AObjectSystem::save(const AObject &object) {
    return toJson(object).save();
}

AJson AObjectSystem::toJson(const AObject &object) {
    AJson result;
    result.setType(AJson::OBJECT);
    result.appendProperty(object.reference(),   "Reference");
    result.appendProperty(object.name(),        "Name");
    result.appendProperty((int)object.id(),     "ID");
    result.appendProperty(object.isEnable(),    "Enable");

    {
        AJson &s    = result.property("Properties");
        s.setType(AJson::ARRAY);
        for(const auto &it : object.getProperties()) {
            AJson p;
            p.setType(AJson::OBJECT);
            p.appendProperty(it.first,          "Name");
            p.appendProperty(it.second.flags,   "Flags");
            p.appendProperty(it.second.data,    "Data");
            s.appendProperty(p);
        }
    }
    {
        AJson &s    = result.property("Signals");
        s.setType(AJson::ARRAY);
        for(const auto &it : object.getSignals()) {
            s.appendProperty(it);
        }
    }
    {
        AJson &s    = result.property("Slots");
        s.setType(AJson::ARRAY);
        for(const auto &it : object.getSlots()) {
            s.appendProperty(it.first);
        }
    }
    {
        AJson &s    = result.property("Components");
        s.setType(AJson::OBJECT);
        for(const auto &it : object.getComponents()) {
            if(it.second) {
                s.appendProperty(toJson(it.second), it.first);
            } else {
                s.appendProperty(static_cast<AObject *>(NULL), it.first);
            }
        }
    }

    return result;
}

AObject *AObjectSystem::createObject(const string &uri, AObject *parent) {
    AObject *pObject  = 0;
    factory_map::iterator it    = mFactories.find(uri);
    if(it != mFactories.end()) {
        pObject = ((*it).second)(parent);
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
