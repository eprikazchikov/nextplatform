#ifndef TST_COMMON_H
#define TST_COMMON_H

#include "aobject.h"

#include "aobjectsystem.h"

#define TSLOT       "testSlot"
#define TVALUE      "testValue"
#define TSIGNAL     "testSignal"
#define TPROPERTY1  "testProperty1"
#define TPROPERTY2  "testProperty2"

class ATestObject : public AObject {
    ACLASS(ATestObject)
    AREGISTER(ATestObject, Test)

public:
    ATestObject     () {
        APROPERTY(TPROPERTY1, &m_bSlot,     AProperty::READ | AProperty::WRITE, AProperty::NONE, -1)
        APROPERTY(TPROPERTY2, &m_Vector2,   AProperty::READ | AProperty::WRITE, AProperty::NONE, -1)

        ASIGNAL(TSIGNAL)
        ASLOT(TSLOT, ATestObject::testSlot)

        m_bSlot         = false;
        m_Vector2       = AVector2D(1, 0);
    }

    static void     testSlot        (AObject *pThis, const AVariant &args) {
        (static_cast<ATestObject *>(pThis))->m_bSlot  = true;
    }

    bool            m_bSlot;
    AVector2D       m_Vector2;
};

#endif // TST_COMMON_H
