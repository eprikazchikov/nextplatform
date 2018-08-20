#ifndef TST_COMMON_H
#define TST_COMMON_H

#include "object.h"

#include "objectsystem.h"

#include <QTest>

class TestObject : public Object {
    A_REGISTER(TestObject, Object, Test)

    A_METHODS(
        A_SLOT(setSlot),
        A_SIGNAL(signal)
    )

    A_PROPERTIES(
        A_PROPERTY(bool, slot, getSlot, setSlot),
        A_PROPERTY(Vector2, vec, getVector, setVector),
        A_PROPERTY(TestObject *, resource, getResource, setResource)
    )

public:
    explicit TestObject     () :
            Object() {
        m_bSlot     = false;
        m_Vector2   = Vector2(1.0f, 0.0f);
        m_pResource = nullptr;
    }

    bool            getSlot         () const {
        return m_bSlot;
    }

    void            setSlot         (const bool value) {
        m_bSlot     = value;
    }

    Vector2         getVector       () const {
        return m_Vector2;
    }

    void            setVector       (const Vector2 &value) {
        m_Vector2   = value;
    }

    TestObject     *getResource     () const {
        return m_pResource;
    }

    void            setResource     (TestObject *resource) {
        m_pResource = resource;
    }

    void            signal          (const bool);

    bool            m_bSlot;
    Vector2         m_Vector2;
    TestObject     *m_pResource;
};

inline bool compare(const Object::Link &left, const Object::Link &right) {
    bool result = true;
    result &= left.signal   == right.signal;
    result &= left.method   == right.method;
    return result;
}


inline bool compare(const Object &left, const Object &right) {
    bool result = true;

    result &= (left.typeName() == right.typeName());

    if(!result) {
        return result;
    }

    {
        for(int i = 0; i < left.metaObject()->propertyCount(); i++) {
            MetaProperty lp     = left.metaObject()->property(i);
            MetaProperty rp     = right.metaObject()->property(i);
            Variant lv  = lp.read(&left);
            Variant rv  = rp.read(&right);
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
        if(left.getChildren().size() == right.getChildren().size()) {
            auto il = left.getChildren().begin();
            auto ir = right.getChildren().begin();
            while(il != left.getChildren().end() && ir != right.getChildren().end()) {
                Object *l  = *il;
                Object *r  = *ir;
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
