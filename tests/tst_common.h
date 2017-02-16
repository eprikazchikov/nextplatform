#ifndef TST_COMMON_H
#define TST_COMMON_H

#include "aobject.h"

#include "aobjectsystem.h"

#include <QTest>

class ATestObject : public AObject {
    A_OBJECT(ATestObject, AObject)
    A_REGISTER(ATestObject, Test)

    A_METHODS(
        A_SLOT(setSlot),
        A_SIGNAL(signal)
    )

    A_PROPERTIES(
        A_PROPERTY(bool, slot, getSlot, setSlot, General),
        A_PROPERTY(AVector2D, vec, getVector, setVector, General)
    )

public:
    explicit ATestObject     (AObject *parent = nullptr) :
            AObject(parent) {
        m_bSlot     = false;
        m_Vector2   = AVector2D(1.0f, 0.0f);
    }

    bool            getSlot         () const {
        return m_bSlot;
    }

    void            setSlot         (const bool value) {
        m_bSlot     = value;
    }

    AVector2D       getVector       () const {
        return m_Vector2;
    }

    void            setVector       (const AVector2D &value) {
        m_Vector2   = value;
    }

    void            signal          (const bool value);

    bool            m_bSlot;
    AVector2D       m_Vector2;
};

#endif // TST_COMMON_H
