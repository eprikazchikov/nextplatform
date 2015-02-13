#ifndef AOBJECTSYSTEM_H
#define AOBJECTSYSTEM_H

#include <unordered_map>
#include <string>

class AObject;
class AVariant;

using namespace std;

// callback_object_factory
typedef AObject            *(*handler_callback)     (AObject *);
typedef unordered_map<string, handler_callback>     factory_map;

class AObjectSystem {
public:
    static AObjectSystem               *instance                ();
    static void                         destroy                 ();

    AObject                            *createObject            (const string &uri, AObject *parent = 0);

    void                                factoryAdd              (const string &uri, handler_callback callback);
    void                                factoryRemove           (const string &uri);
    void                                factoryClear            ();

    bool                                factoryFirst            (string &uri);
    bool                                factoryNext             (string &uri);

    unsigned int                        nextId                  ();

    const string                       &systemName              () const;
    void                                setSytemName            (const string &name);

private:
    AObjectSystem                       ();
    ~AObjectSystem                      ();

    static AObjectSystem               *m_pInstance;

    /// Container for registered callbacks.
    factory_map                         mFactories;

    factory_map::iterator               fIterator;

    unsigned int                        mNextId;

    string                              mSystemName;
};

#endif // AOBJECTSYSTEM_H
