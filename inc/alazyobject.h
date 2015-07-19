#ifndef ALAZYOBJECT_H
#define ALAZYOBJECT_H

#include "aobject.h"

class ALazyObject : public AObject {
public:
    ALazyObject                 (AObject *prototype);
    virtual ~ALazyObject        ();

    AObject                    *prototype                   () const;

public:
    AVariant                    property                    (const string &name);
    AProperty                   propertySettings            (const string &name);

    objectsMap                  getComponents               () const;
    propertiesMap               getProperties               () const;
    stringVector                getSignals                  () const;
    slotsMap                    getSlots                    () const;
    linksList                   getLinks                    () const;

    void                        addLink                     (link_data &link);
    void                        addSignal                   (const string &signal);
    void                        addSlot                     (const string &slot, callback ptr);

    AObject                    *findObject                  (const string &path);

    const string                typeName                    () const;

private:
    /// Prototype object
    AObject                    *m_pPrototype;

    string                      m_sType;
};

#endif // ALAZYOBJECT_H
