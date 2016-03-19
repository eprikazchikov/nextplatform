#ifndef AOBJECTSYSTEM_H
#define AOBJECTSYSTEM_H

#include <unordered_map>
#include <string>

class AObject;
class AVariant;

using namespace std;

typedef unordered_map<string, AObject *>   factoryMap;
typedef unordered_map<string, string>     groupMap;

class AObjectSystem {
public:
    AObjectSystem                       ();
    ~AObjectSystem                      ();

    AObject                            *objectCreate            (const string &url, AObject *parent = 0);

    void                                factoryAdd              (const string &uri, AObject *prototype);
    void                                factoryRemove           (const string &uri);
    void                                factoryClear            ();

    groupMap                            factory                 () const;

    const string                       &systemName              () const;
    void                                setSytemName            (const string &name);

private:
    /// Container for registered callbacks.
    factoryMap                          mFactories;
    groupMap                            mGroups;

    string                              mSystemName;
};

#endif // AOBJECTSYSTEM_H
