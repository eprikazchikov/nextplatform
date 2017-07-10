#include "aobjectsystem.h"

#include "aobject.h"
#include "auri.h"
#include "abson.h"
#include "ajson.h"

AObjectSystem *AObjectSystem::s_Instance    = nullptr;

AObjectSystem::AObjectSystem(const string &name) :
        m_Exit(false) {
    PROFILE_FUNCTION()
    if(AObjectSystem::s_Instance != nullptr) {
        throw "There should be only one ObjectSystem object";
    }
    AObjectSystem::s_Instance   = this;
    m_sName     = name;
    m_NextID    = 1000;
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
    AObject *object = nullptr;

    AObjectSystem *inst = instance();
    FactoryMap::iterator it = inst->m_Factories.find(uri);
    if(it == inst->m_Factories.end()) {
        it  = inst->m_Factories.find(inst->m_Groups[uri]);
    }
    if(it != inst->m_Factories.end()) {
        object = (*it).second->createInstance();
        object->setName(name);
        object->setParent(parent);
        object->m_UUID  = inst->m_NextID++;
    }
    return object;
}

void AObjectSystem::factoryClear() {
    PROFILE_FUNCTION()
    m_Factories.clear();
}

AObjectSystem::GroupMap AObjectSystem::factories() const {
    PROFILE_FUNCTION()
    return m_Groups;
}

bool AObjectSystem::isObject(const string &typeName) {
    PROFILE_FUNCTION()
    auto it = m_TypeSet.find(typeName);
    if(it != m_TypeSet.end()) {
        return true;
    }
    return false;
}

typedef list<const AObject *> ObjectArray;

void enumObjects(const AObject *object, ObjectArray &list) {
    PROFILE_FUNCTION()
    list.push_back(object);
    for(const auto &it : object->getChildren()) {
        enumObjects(it, list);
    }
}

AVariant AObjectSystem::toVariant(const AObject *object) {
    PROFILE_FUNCTION()
    AVariant::AVariantList result;

    ObjectArray array;
    enumObjects(object, array);

    for(auto it : array) {
        // Save Object
        int uuid    = int(it->uuid());

        AVariant::AVariantList o;
        o.push_back(uuid);
        AObject *parent = it->parent();
        o.push_back(int((parent) ? parent->uuid() : 0));
        o.push_back(it->typeName());
        o.push_back(it->name());
        o.push_back(it->isEnable());

        // Save base properties
        AVariant::AVariantMap properties;
        const AMetaObject *meta = it->metaObject();
        for(uint32_t i = 0; i < meta->propertyCount(); i++) {
            AMetaProperty p = meta->property(i);
            if(p.isValid()) {
                AVariant v  = p.read(it);
                if(v.userType() < AMetaType::UserType) {
                    properties[p.name()] = v;
                }
            }
        }
        for(const auto it : it->getDynamicProperties()) {
            if(it.second.userType() < AMetaType::UserType) {
                properties[it.first]     = it.second;
            }
        }

        // Save links
        AVariant::AVariantList links;
        for(const auto &l : it->getSenders()) {
            AVariant::AVariantList link;

            AObject *sender   = l.sender;

            link.push_back(int(sender->uuid()));
            AMetaMethod method  = sender->metaObject()->method(l.signal);
            link.push_back(AVariant(char(method.type() + 0x30) + method.signature()));

            link.push_back(uuid);
            method      = it->metaObject()->method(l.method);
            link.push_back(AVariant(char(method.type() + 0x30) + method.signature()));

            links.push_back(link);
        }
        o.push_back(properties);
        o.push_back(object->saveUserData());
        o.push_back(links);

        result.push_back(o);
    }

    return result;
}

AObject *AObjectSystem::toObject(const AVariant &variant) {
    PROFILE_FUNCTION()
    AObject *result = nullptr;

    // Create all declared objects
    AVariant::AVariantList objects  = variant.value<AVariant::AVariantList>();
    objects.pop_front();

    ObjectMap array;
    for(auto it : objects) {
        AVariant::AVariantList o    = it.value<AVariant::AVariantList>();

        auto i      = o.begin();
        string uuid = (*i).toString();
        i++;
        AObject *parent = nullptr;
        auto a  = array.find((*i).toString());
        if(a != array.end()) {
            parent  = (*a).second;
        }
        i++;
        string type = (*i).toString();
        i++;
        string name = (*i).toString();
        i++;
        bool enable = (*i).toBool();
        i++;

        AObject *object = objectCreate(type, name, parent);
        if(object) {
            if(!object->parent()) {
                result  = object;
            }
            object->setEnable(enable);
            array[uuid] = object;
            // Load base properties
            for(const auto &it : (*i).toMap()) {
                AVariant v  = it.second;
                if(v.type() < AMetaType::UserType) {
                    object->setProperty(it.first.c_str(), v);
                }
            }
            i++;
        }
    }
    // Restore connections
    for(auto it : objects) {
        AVariant::AVariantList o    = it.value<AVariant::AVariantList>();
        AVariant::AVariantList list = o.back().value<AVariant::AVariantList>();
        for(const auto &link : list) {
            AVariant::AVariantList l = link.value<AVariant::AVariantList>();
            AObject *sender     = nullptr;
            AObject *receiver   = nullptr;

            auto i  = l.begin();
            auto s = array.find((*i).toString());
            if(s != array.end()) {
                sender  = (*s).second;
            }
            i++;

            string signal = (*i).toString();
            i++;

            s = array.find((*i).toString());
            if(s != array.end()) {
                receiver  = (*s).second;
            }
            i++;

            string method = (*i).toString();
            i++;

            connect(sender, signal.c_str(), receiver, method.c_str());
        }
    }
    return result;
}
