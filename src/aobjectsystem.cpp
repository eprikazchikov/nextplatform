#include "aobjectsystem.h"

#include "aobject.h"
#include "auri.h"

AObjectSystem *AObjectSystem::s_Instance    = nullptr;

AObjectSystem::AObjectSystem(const string &name) {
    if(AObjectSystem::s_Instance != nullptr) {
        throw "There should be only one ObjectSystem object";
    }
    AObjectSystem::s_Instance   = this;
    m_sName     = name;
}

AObjectSystem::~AObjectSystem() {
    factoryClear();
    AObjectSystem::s_Instance   = nullptr;
}

int32_t AObjectSystem::exec() {
    return 0;
}

AObjectSystem *AObjectSystem::instance() {
    return AObjectSystem::s_Instance;
}

AObject *AObjectSystem::objectCreate(const string &uri, AObject *parent) {
    AObject *object = 0;
    FactoryMap::iterator it = AObjectSystem::s_Instance->m_Factories.find(uri);
    if(it == AObjectSystem::s_Instance->m_Factories.end()) {
        it  = AObjectSystem::s_Instance->m_Factories.find(AObjectSystem::s_Instance->m_Groups[uri]);
    }
    if(it != AObjectSystem::s_Instance->m_Factories.end()) {
        object = (*it).second->createInstance(parent);
    }

    return object;
}

void AObjectSystem::factoryAdd(const string &uri, const AMetaObject *meta) {
    AUri group(uri);
    AObjectSystem::s_Instance->m_Groups[group.name()]   = uri;
    AObjectSystem::s_Instance->m_Factories[uri]         = meta;
}

void AObjectSystem::factoryRemove(const string &uri) {
    AObjectSystem::s_Instance->m_Factories.erase(uri);
}

void AObjectSystem::factoryClear() {
    m_Factories.clear();
}

AObjectSystem::GroupMap AObjectSystem::factories() const {
    return m_Groups;
}
