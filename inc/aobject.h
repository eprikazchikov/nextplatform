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

#define APROPERTY(object, name, value, flags) object->setProperty(name, value); object->setPropertyFlags(name, flags);

using namespace std;

class AObject {
public:
    enum AccessTypes {
        NONE                    = 0,
        READ                    = (1<<0),
        WRITE                   = (1<<1),
        CONSTANT                = (1<<2)
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
    };

    typedef map<string, AObject *>  objects_map;

    typedef vector<AVariant>        variant_vector;
    typedef list<link_data>         links_list;

    typedef void                (*callback)                 (AObject *pThis, const variant_vector &args);

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
    AObject                     (AObject *parent);
    virtual ~AObject            ();

    unsigned int                id                          () const;
    AObject                    *parent                      () const;
    AObject                    *component                   (string &name);
    string                      name                        () const;

    static void                 addEventListner             (AObject *sender, const string &signal, AObject *receiver, const string &slot);
    static void                 removeEventListner          (AObject *sender, const string &signal, AObject *receiver, const string &slot);

    void                        addLink                     (link_data &link);
    void                        removeLink                  (link_data &link);

    void                        deleteLater                 ();

    objects_map                 getComponents               () const;
    properties_map              getProperties               () const;
    string_vector               getSignals                  () const;
    slots_map                   getSlots                    () const;
    links_list                  getLinks                    () const;

    void                        setId                       (unsigned int id);
    void                        setParent                   (AObject *parent);
    void                        setName                     (const string &value);
    void                        setComponent                (const string &name, AObject *value);

    bool                        isEnable                    () const;

// Virtual members
public:
    virtual bool                update                      (float dt);

    virtual void                dispatchEvent               (const string &name, const variant_vector &args);

    virtual void                emitSignal                  (const string &name, const variant_vector &args);

    virtual AVariant            property                    (const string &name);

    static string               reference                   ();

    virtual string              typeName                    () const;

    virtual void                setEnable                   (bool state);

    virtual void                setProperty                 (const string &name, const AVariant &value);
    void                        setPropertyFlags            (const string &name, const int flags);

    virtual void                load                        (/*const xml_node &node*/);
    virtual void                save                        (/*xml_node &node*/);
};

#endif // AOBJECT_H
