#ifndef AOBJECT_P_H
#define AOBJECT_P_H

#include "aobject.h"

class AObjectPrivate {
public:
    AObjectPrivate() :
        m_bEnable(true),
        m_pParent(nullptr),
        m_pCurrentSender(nullptr),
        m_UUID(0) {

    }

    /// Enable object flag
    bool                            m_bEnable;
    /// Parent object
    AObject                        *m_pParent;
    /// Object name
    string                          m_sName;

    AObject::ObjectList             m_mChildren;
    AObject::LinkList               m_lRecievers;
    AObject::LinkList               m_lSenders;

    AObject                        *m_pCurrentSender;

    typedef queue<AEvent *>         EventQueue;
    EventQueue                      m_EventQueue;

    uint32_t                        m_UUID;

    mutex                           m_Mutex;

};

#endif // AOBJECT_P_H
