#include "tst_objectsystem.h"

#include "tst_common.h"

#include "aobjectsystem.h"

#include "ajson.h"

#include <QtTest>

void ObjectSystemTest::Object_Instansing() {
    ATestObject obj1;

    AObject *result = AObjectSystem::objectCreate<ATestObject>();
    AObject *object = dynamic_cast<AObject*>(&obj1);

    QCOMPARE((result != 0), true);
    QCOMPARE((object != 0), true);
    QCOMPARE(compare(*object, *result), true);

    delete result;
}

void ObjectSystemTest::Serialize_Desirialize_Object() {
    ATestObject obj1;
    ATestObject obj2;
    ATestObject obj3;

    obj1.setName("MainObject");
    obj2.setName("TestComponent2");
    obj3.setName("TestComponent3");
    obj2.setParent(&obj1);
    obj3.setParent(&obj1);
    obj1.setProperty("DynamicProperty", true);

    AObject::connect(&obj1, _SIGNAL(signal(bool)), &obj2, _SLOT(setSlot(bool)));
    AObject::connect(&obj1, _SIGNAL(signal(bool)), &obj3, _SIGNAL(signal(bool)));
    AObject::connect(&obj2, _SIGNAL(signal(bool)), &obj3, _SLOT(setSlot(bool)));

    AVariant data   = obj1.toVariant();
    AObject *result = AObject::toObject(data);
    AObject *object = dynamic_cast<AObject*>(&obj1);

    QCOMPARE((result != 0), true);
    QCOMPARE((object != 0), true);
    QCOMPARE(compare(*object, *result), true);

    delete result;
}

void ObjectSystemTest::Delta_Serialize_Desirialize_Object() {
/*
    AObject *instance   = m_pSystem->objectCreate(ATestObject::metaClass()->name());
    AObject *obj1       = m_pSystem->objectCreate(ATestObject::metaClass()->name());
    instance->addComponent("TestComponent1", obj1);

    /// \todo: Links referencies destroy deltas if name changed
    AObject::addEventListner(instance, TSIGNAL, obj1, TSLOT);
    AObject::addEventListner(instance, TPROPERTY1, obj1, TPROPERTY1);

    instance->setName("CopiedObject");
    instance->setProperty(TPROPERTY1, true);
    instance->setProperty("Dynamic", 5);

    AVariant data   = instance->toVariant();
    AObject *result = AObject::toObject(data, m_pSystem);

    QCOMPARE((result != 0), true);
    QCOMPARE(result->property(TPROPERTY1).toBool(), true);
    QCOMPARE((*instance == *result), true);

    delete result;

    delete instance;
*/
}

void ObjectSystemTest::Virtual_Inheritance() {
/*
    AObject *prototype  = m_pSystem->objectCreate(ATestObject::metaClass()->name());
    prototype->setName("PrototypeObject");

    AObject *obj1       = m_pSystem->objectCreate(ATestObject::metaClass()->name());
    prototype->addComponent("TestComponent1", obj1);
    prototype->setType("InheritedClass");

    m_pSystem->factoryAdd(prototype->typeName(), prototype);

    AObject *instance   = m_pSystem->objectCreate(prototype->typeName());
    instance->setName("CopiedObject");

    AVariant data   = instance->toVariant();
    AObject *result = AObject::toObject(data, m_pSystem);

    QCOMPARE((result != 0), true);
    QCOMPARE(result->property(TPROPERTY1).toBool(), false);
    QCOMPARE((*instance == *result), true);

    delete instance;
    delete result;
*/
}
