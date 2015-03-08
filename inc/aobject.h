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

#define APROPERTY(name, value, flags, type, order) \
{\
    setProperty(name, value);\
    setPropertySettings(name, flags, type, order);\
}

#define ASIGNAL(name) \
{\
    m_mSignals.push_back(name);\
}

#define ASLOT(name, callback) \
{\
    m_mSlots[name] = &callback;\
}

using namespace std;

class AObject {
public:
    enum AccessTypes {
        NONE                    = 0,
        READ                    = (1<<0),
        WRITE                   = (1<<1),
        SCHEME                  = (1<<2),
        EDITOR                  = (1<<3)
    };

    enum UserTypes {
        FILEPATH                = 1,
        COLOR
    };

    struct link_data {
        AObject                *sender;

        string                  signal;

        AObject                *receiver;

        string                  slot;
    };

    struct property_data {
        AVariant                data;

        AccessTypes             flags;

        int                     type;

        int                     order;
    };

    typedef map<string, AObject *>  objects_map;

    typedef vector<AVariant>        variant_vector;
    typedef list<link_data>         links_list;

    typedef void                (*callback)        (AObject *pThis, const variant_vector &args);

    typedef map<string, property_data>  properties_map;
    typedef map<string, callback>       slots_map;
    typedef vector<string>              string_vector;

protected:
    /// Enable object flag
    bool                        m_bEnable;
    /// Remove object flag
    bool                        m_bDelete;

    /// Object ID
    unsigned int                m_id;
    /// Object name
    string                      m_sName;

    objects_map                 m_mComponents;
    links_list                  m_lLinks;

    properties_map              m_mProperties;
    string_vector               m_mSignals;
    slots_map                   m_mSlots;

    /// Parent object
    AObject                    *m_pParent;

public:
    AObject                     (AObject *parent = 0);
    virtual ~AObject            ();

    unsigned int                id                          () const;
    AObject                    *parent                      () const;
    AObject                    *component                   (string &name);
    string                      name                        () const;

    string                      reference                   () const;

    static void                 addEventListner             (AObject *sender, const string &signal, AObject *receiver, const string &slot);
    static void                 removeEventListner          (AObject *sender, const string &signal, AObject *receiver, const string &slot);

    void                        addLink                     (const link_data &link);
    void                        removeLink                  (const link_data &link);

    void                        deleteLater                 ();

    objects_map                 getComponents               () const;
    properties_map              getProperties               () const;
    string_vector               getSignals                  () const;
    slots_map                   getSlots                    () const;
    links_list                  getLinks                    () const;
    AVariant                    getLinks                    (const string &name) const;

    AObject                    *findObject                  (const string &path);

    void                        setParent                   (AObject *parent);
    void                        setName                     (const string &value);
    void                        setComponent                (const string &name, AObject *value);

    bool                        isEnable                    () const;

    static AObject             *callbackClassFactory        (AObject *parent = 0);

    static void                 registerClassFactory        ();

    static AVariant             toVariant                   (const AObject &object);

    static AObject             *toObject                    (const AVariant &variant, AObject *parent = 0);

    bool                        operator==                  (const AObject &right) const;
    bool                        operator!=                  (const AObject &right) const;

// Virtual members
public:
    virtual bool                update                      (float dt);

    virtual void                dispatchEvent               (const string &name, const variant_vector &args);

    virtual void                onEvent                     (const string &name, const variant_vector &args);

    virtual void                emitSignal                  (const string &name, const variant_vector &args);

    virtual AVariant            property                    (const string &name);

    virtual string              typeName                    () const;

    virtual void                setEnable                   (bool state);

    virtual void                setProperty                 (const string &name, const AVariant &value);
    inline void                 setPropertySettings         (const string &name, const int flags, const int type = 0, const int order = -1);
};

inline bool                     operator==                  (const AObject::property_data &left, const AObject::property_data &right);
inline bool                     operator==                  (const AObject::link_data &left, const AObject::link_data &right);

#endif // AOBJECT_H
