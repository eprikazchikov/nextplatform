#ifndef AOBJECTSYSTEM_H
#define AOBJECTSYSTEM_H

#include <unordered_map>
#include <string>
#include <memory>

#include "aobject.h"

class AMetaObject;

using namespace std;

class AObjectSystem : public AObject{
public:
    typedef unordered_map<string, const AMetaObject *>  FactoryMap;
    typedef unordered_map<string, string>               GroupMap;

public:
    AObjectSystem                       (const string &name = "system");
    ~AObjectSystem                      ();

    virtual int32_t                     exec                    ();

    static AObjectSystem               *instance                ();

    static AObject                     *objectCreate            (const string &url, AObject *parent = 0);

    static void                         factoryAdd              (const string &uri, const AMetaObject *meta);
    static void                         factoryRemove           (const string &uri);

    GroupMap                            factories               () const;

private:
    friend class ObjectSystemTest;

    void                                factoryClear            ();

    /// Container for registered callbacks.
    FactoryMap                          m_Factories;
    GroupMap                            m_Groups;

    static AObjectSystem               *s_Instance;

};

#endif // AOBJECTSYSTEM_H
