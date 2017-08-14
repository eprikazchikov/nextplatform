#ifndef TST_COMMON_H
#define TST_COMMON_H

#include "aobject.h"

#include "aobjectsystem.h"

#include <QTest>

class ATestObject : public AObject {
    A_REGISTER(ATestObject, AObject, Test)

    A_METHODS(
        A_SLOT(setSlot),
        A_SIGNAL(signal)
    )

    A_PROPERTIES(
        A_PROPERTY(bool, slot, getSlot, setSlot),
        A_PROPERTY(AVector2D, vec, getVector, setVector),
        A_PROPERTY(ATestObject *, resource, getResource, setResource)
    )

public:
    explicit ATestObject     () :
            AObject() {
        m_bSlot     = false;
        m_Vector2   = AVector2D(1.0f, 0.0f);
        m_pResource = nullptr;
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

    ATestObject    *getResource     () const {
        return m_pResource;
    }

    void            setResource     (ATestObject *resource) {
        m_pResource = resource;
    }

    void            signal          (const bool value);

    bool            m_bSlot;
    AVector2D       m_Vector2;
    ATestObject    *m_pResource;
};

inline bool compare(const AObject::Link &left, const AObject::Link &right) {
    bool result = true;
    result &= left.signal   == right.signal;
    result &= left.method   == right.method;
    return result;
}


inline bool compare(const AObject &left, const AObject &right) {
    bool result = true;

    result &= (left.isEnable()  == right.isEnable());
    result &= (left.typeName()  == right.typeName());

    if(!result) {
        return result;
    }

    {
        for(int i = 0; i < left.metaObject()->propertyCount(); i++) {
            AMetaProperty lp    = left.metaObject()->property(i);
            AMetaProperty rp    = right.metaObject()->property(i);
            AVariant lv = lp.read(&left);
            AVariant rv = rp.read(&right);
            if(lp.name() != rp.name() || lv != rv) {
                return false;
            }
        }
    }

    {
        if(left.getReceivers().size() == right.getReceivers().size()) {
            for(const auto &li : left.getReceivers()) {
                result  = false;
                for(const auto &ri : right.getReceivers()) {
                    if(compare(li, ri)) {
                        result  = true;
                        break;
                    }
                }
                if(!result) {
                    return result;
                }
            }
        } else {
            return false;
        }
    }

    {
        if(left.getSenders().size() == right.getSenders().size()) {
            for(const auto &li : left.getSenders()) {
                result  = false;
                for(const auto &ri : right.getSenders()) {
                    if(compare(li, ri)) {
                        result  = true;
                        break;
                    }
                }
                if(!result) {
                    return result;
                }
            }
        } else {
            return false;
        }
    }

    {
        if(left.getChildren().size() == right.getChildren().size()) {
            auto il = left.getChildren().begin();
            auto ir = right.getChildren().begin();
            while(il != left.getChildren().end() && ir != right.getChildren().end()) {
                AObject *l  = *il;
                AObject *r  = *ir;
                if(!compare(*l, *r)) {
                    return false;
                }
                il++;
                ir++;
            }
        } else {
            return false;
        }
    }

    return true;
}

#endif // TST_COMMON_H
