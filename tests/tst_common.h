#ifndef TST_COMMON_H
#define TST_COMMON_H

#include "aobject.h"

#include "aobjectsystem.h"

#define TSLOT       "testSlot"
#define TVALUE      "testValue"
#define TSIGNAL     "testSignal"
#define TPROPERTY   "testProperty"

class ATestObject : public AObject {
public:
    ATestObject     (AObject *parent = 0) :
            AObject(parent) {
        APROPERTY(TPROPERTY, &m_bSlot, AObject::READ | AObject::WRITE, NONE, -1)
        ASIGNAL(TSIGNAL)
        ASLOT(TSLOT, ATestObject::testSlot)

        m_bSlot         = false;
    }

    static AObject *callbackClassFactory(AObject *parent = 0) {
        return new ATestObject(parent);
    }

    static void     registerClassFactory() {
        ATestObject object;
        AObjectSystem::instance()->factoryAdd(object.typeName(), &ATestObject::callbackClassFactory);
    }

    string          typeName        () const {
        return "ATestObject";
    }

    static void     testSlot        (AObject *pThis, const variant_vector &args) {
        (static_cast<ATestObject *>(pThis))->m_bSlot  = true;
    }

    bool            m_bSlot;
};

#endif // TST_COMMON_H
