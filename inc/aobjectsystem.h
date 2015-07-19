#ifndef AOBJECTSYSTEM_H
#define AOBJECTSYSTEM_H

#include <unordered_map>
#include <string>

class AObject;
class AVariant;

using namespace std;

typedef unordered_map<string, AObject *>    factoryMap;
typedef unordered_map<string, string>       groupMap;

class AObjectSystem {
public:
    static AObjectSystem               *instance                ();
    static void                         destroy                 ();

    AObject                            *createObject            (const string &url, AObject *parent = 0);

    void                                factoryAdd              (const string &uri, AObject *prototype);
    void                                factoryRemove           (const string &uri);
    void                                factoryClear            ();

    groupMap                            factory                 () const;

    unsigned int                        nextId                  ();

    const string                       &systemName              () const;
    void                                setSytemName            (const string &name);

private:
    AObjectSystem                       ();
    ~AObjectSystem                      ();

    static AObjectSystem               *m_pInstance;

    /// Container for registered callbacks.
    factoryMap                          mFactories;
    groupMap                            mGroups;

    unsigned int                        mNextId;

    string                              mSystemName;
};

#endif // AOBJECTSYSTEM_H
