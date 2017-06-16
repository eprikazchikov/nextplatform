#include "aobjectsystem.h"

#include "aobject.h"
#include "auri.h"

AObjectSystem *AObjectSystem::s_Instance    = nullptr;

AObjectSystem::AObjectSystem(const string &name) :
        m_Exit(false) {
    PROFILE_FUNCTION()
    if(AObjectSystem::s_Instance != nullptr) {
        throw "There should be only one ObjectSystem object";
    }
    AObjectSystem::s_Instance   = this;
    m_sName     = name;
}

AObjectSystem::~AObjectSystem() {
    PROFILE_FUNCTION()
    factoryClear();
    AObjectSystem::s_Instance   = nullptr;
}

int32_t AObjectSystem::exec() {
    PROFILE_FUNCTION()
    while(!m_Exit) {

    }
    return 0;
}

AObjectSystem *AObjectSystem::instance() {
    PROFILE_FUNCTION()
    return AObjectSystem::s_Instance;
}

AObject *AObjectSystem::objectCreate(const string &uri, const string &name, AObject *parent) {
    PROFILE_FUNCTION()
    AObject *object = 0;
    FactoryMap::iterator it = AObjectSystem::s_Instance->m_Factories.find(uri);
    if(it == AObjectSystem::s_Instance->m_Factories.end()) {
        it  = AObjectSystem::s_Instance->m_Factories.find(AObjectSystem::s_Instance->m_Groups[uri]);
    }
    if(it != AObjectSystem::s_Instance->m_Factories.end()) {
        object = (*it).second->createInstance();
        object->setName(name);
        object->setParent(parent);
    }

    return object;
}

void AObjectSystem::factoryAdd(const string &uri, const AMetaObject *meta) {
    PROFILE_FUNCTION()
    AUri group(uri);

    AObjectSystem::s_Instance->m_Groups[group.name()]   = uri;
    AObjectSystem::s_Instance->m_Factories[uri]         = meta;
}

void AObjectSystem::factoryRemove(const string &uri) {
    PROFILE_FUNCTION()
    AUri group(uri);
    AObjectSystem::s_Instance->m_Groups.erase(group.name());
    AObjectSystem::s_Instance->m_Factories.erase(uri);
}

void AObjectSystem::factoryClear() {
    PROFILE_FUNCTION()
    m_Factories.clear();
}

AObjectSystem::GroupMap AObjectSystem::factories() const {
    PROFILE_FUNCTION()
    return m_Groups;
}
