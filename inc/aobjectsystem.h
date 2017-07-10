#ifndef AOBJECTSYSTEM_H
#define AOBJECTSYSTEM_H

#include <unordered_map>
#include <set>
#include <string>
#include <memory>

#include "aobject.h"

class AMetaObject;

using namespace std;

class NEXT_LIBRARY_EXPORT AObjectSystem : public AObject {
public:
    typedef unordered_map<string, AObject *>            ObjectMap;
    typedef unordered_map<string, const AMetaObject *>  FactoryMap;
    typedef unordered_map<string, string>               GroupMap;
    typedef set<string>                                 TypeSet;

public:
    AObjectSystem                       (const string &name = "system");
    ~AObjectSystem                      ();

    virtual int32_t                     exec                    ();

    static AObjectSystem               *instance                ();

    static AObject                     *objectCreate            (const string &uri, const string &name = string(), AObject *parent = 0);

    template<typename T>
    static T                           *objectCreate            (const string &name = string(), AObject *parent = 0) {
        return dynamic_cast<T *>(objectCreate(T::metaClass()->name(), name, parent));
    }

    template<typename T>
    static void                         factoryAdd              (const string &group, const AMetaObject *meta) {
        string name = T::metaClass()->name();
        string uri  = string("thor://") + group + "/" + name;
        AObjectSystem *inst = AObjectSystem::instance();
        inst->m_Groups[name]    = uri;
        inst->m_Factories[uri]  = meta;

        name += " *";
        if(AMetaType::type(name.c_str()) == 0) {
            registerMetaType<T *>(name.c_str());
            inst->m_TypeSet.insert(name);
        }
    }

    template<typename T>
    static void                         factoryRemove           (const string &group) {
        const char *name    = T::metaClass()->name();
        string uri  = string("thor://") + group + "/" + name;
        AObjectSystem *inst = AObjectSystem::instance();
        inst->m_Groups.erase(name);
        inst->m_Factories.erase(uri);
    }

    GroupMap                            factories               () const;

    bool                                isObject                (const string &typeName);

    static AVariant                     toVariant               (const AObject *object);
    static AObject                     *toObject                (const AVariant &variant);

private:
    friend class ObjectSystemTest;

    void                                factoryClear            ();

    /// Container for registered callbacks.
    FactoryMap                          m_Factories;
    GroupMap                            m_Groups;

    TypeSet                             m_TypeSet;

    bool                                m_Exit;

    uint32_t                            m_NextID;

    static AObjectSystem               *s_Instance;
};

#endif // AOBJECTSYSTEM_H
