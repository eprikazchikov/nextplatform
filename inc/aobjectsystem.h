#ifndef AOBJECTSYSTEM_H
#define AOBJECTSYSTEM_H

#include <unordered_map>

#include <aobject.h>

// callback_object_factory
typedef AObject            *(*handler_callback)     (AObject *);
typedef unordered_map<string, handler_callback>     factory_map;

class AObjectSystem {
public:
    static AObjectSystem               *instance                ();
    static void                         destroy                 ();

    void                                clear                   ();

    AObject                            *createObject            (const string &uri, AObject *parent = 0);

    void                                factoryAdd              (const string &uri, handler_callback callback);
    void                                factoryRemove           (const string &uri);
    void                                factoryClear            ();

    bool                                factoryFirst            (string &uri);
    bool                                factoryNext             (string &uri);

private:
    AObjectSystem                       ();
    ~AObjectSystem                      ();

    static AObjectSystem               *m_pInstance;

    /// Container for registered callbacks.
    factory_map                         mFactories;

    factory_map::iterator               fIterator;
};

#endif // AOBJECTSYSTEM_H
