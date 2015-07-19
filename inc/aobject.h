/*
    This file is part of Thunder Next.

    Thunder Next is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Thunder Next is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Thunder Next.  If not, see <http://www.gnu.org/licenses/>.

    © Copyright: 2008-2014 Evgeny Prikazchikov
*/

#ifndef AOBJECT_H
#define AOBJECT_H

#include <string>
#include <map>
#include <list>

#include <avariant.h>
#include <aobjectsystem.h>

#define ACLASS(name) \
public: \
    virtual const string        typeName                    () const { return typeNameS(); } \
    static const string         typeNameS                   () { return #name; }

#ifndef AREGISTER
#define AREGISTER(name, group) \
public: \
    static void                 registerClassFactory        () { \
        AObjectSystem::instance()->factoryAdd(string("thor://") + #group + "/" + name::typeNameS(), new name()); \
    }
#endif

#define APROPERTY(name, value, flags, type, order) \
{\
    setProperty(name, value);\
    setPropertySettings(name, flags, type, order);\
}

#define ASIGNAL(name)           m_mSignals.push_back(name);
#define ASLOT(name, callback)   m_mSlots[name] = &callback;

using namespace std;

class AProperty {
public:
    enum AccessTypes {
        NONE                    = 0,
        READ                    = (1<<0),
        WRITE                   = (1<<1),
        SCHEME                  = (1<<2),
        EDITOR                  = (1<<3),
        NATIVE                  = (1<<4)
    };

    enum UserTypes {
        FILEPATH                = 1,
        COLOR
    };

public:
    AProperty                   () {
        flags   = (AccessTypes)(READ | WRITE);
        type    = NONE;
        order   = -1;
    }

public:
    AVariant                    data;

    AccessTypes                 flags;

    int                         type;

    int                         order;
};

class AObject {
    ACLASS(AObject)
    AREGISTER(AObject, General)

public:
    struct link_data {
        AObject                *sender;

        string                  signal;

        AObject                *receiver;

        string                  slot;
    };

    typedef map<string, AObject *>      objectsMap;
    typedef list<AObject *>             objectsList;

    typedef list<link_data>             linksList;


    typedef void                (*callback)        (AObject *pThis, const AVariant &args);

    typedef map<string, AProperty>      propertiesMap;
    typedef map<string, callback>       slotsMap;
    typedef vector<string>              stringVector;
    typedef list<string>                stringList;

protected:
    /// Enable object flag
    bool                        m_bEnable;
    /// Remove object flag
    bool                        m_bDelete;

    bool                        m_bNative;

    /// Object ID
    unsigned int                m_id;
    /// Object name
    string                      m_sName;

    objectsMap                  m_mComponents;
    linksList                   m_lLinks;

    propertiesMap               m_mProperties;
    stringVector                m_mSignals;
    slotsMap                    m_mSlots;

    /// Parent object
    AObject                    *m_pParent;

    objectsList                 m_lModels;

public:
    AObject                     ();

    virtual ~AObject            ();

    unsigned int                id                          () const;
    AObject                    *parent                      () const;
    AObject                    *component                   (string &name);
    string                      name                        () const;

    string                      reference                   () const;

    void                        addModel                    (AObject *model);
    void                        removeModel                 (const AObject *model);

    static void                 addEventListner             (AObject *sender, const string &signal, AObject *receiver, const string &slot);
    static void                 removeEventListner          (AObject *sender, const string &signal, AObject *receiver, const string &slot);

    void                        deleteLater                 ();

    void                        setParent                   (AObject *parent);
    void                        setName                     (const string &value);
    void                        addComponent                (const string &name, AObject *value);

    bool                        isEnable                    () const;
    bool                        isNative                    () const;
    bool                        isLinkExist                 (const link_data &link) const;
    bool                        isPropertyExist             (const string &property) const;
    bool                        isSignalExist               (const string &signal) const;
    bool                        isSlotExist                 (const string &slot) const;

    AVariant                    toVariant                   ();
    void                        fromVariant                 (const AVariant &variant);

    static AObject             *toObject                    (const AVariant &variant, AObject *parent = 0);

    AObject                    &operator=                   (AObject &right);

    bool                        operator==                  (const AObject &right) const;
    bool                        operator!=                  (const AObject &right) const;

// Virtual members
public:
    virtual bool                update                      (float dt);

    virtual void                dispatchEvent               (const string &name, const AVariant &args = AVariant());

    virtual void                onEvent                     (const string &name, const AVariant &args = AVariant());

    virtual void                emitSignal                  (const string &name, const AVariant &args = AVariant());

    virtual AVariant            property                    (const string &name);

    virtual AProperty           propertySettings            (const string &name);

    virtual objectsMap          getComponents               () const;
    virtual propertiesMap       getProperties               () const;
    virtual stringVector        getSignals                  () const;
    virtual slotsMap            getSlots                    () const;
    virtual linksList           getLinks                    () const;

    virtual void                addLink                     (link_data &link);
    virtual void                removeLink                  (const link_data &link);

    virtual void                addSignal                   (const string &signal);
    virtual void                removeSignal                (const string &signal);

    virtual void                addSlot                     (const string &slot, callback ptr);
    virtual void                removeSlot                  (const string &slot);

    virtual AObject            *findObject                  (const string &path);

    virtual void                setEnable                   (bool state);

    virtual void                setProperty                 (const string &name, const AVariant &value);

    virtual void                setPropertySettings         (const string &name, const int flags, const int type = 0, const int order = -1);
};

inline bool                     operator==                  (const AProperty &left, const AProperty &right);
inline bool                     operator!=                  (const AProperty &left, const AProperty &right);

inline bool                     operator==                  (const AObject::link_data &left, const AObject::link_data &right);
inline bool                     operator!=                  (const AObject::link_data &left, const AObject::link_data &right);

#endif // AOBJECT_H
