#include "tst_objectsystem.h"

#include "tst_common.h"

#include "aobjectsystem.h"

#include "ajson.h"

#include <QtTest>

void ObjectSystemTest::initTestCase() {
    m_pSystem   = new AObjectSystem;
    ATestObject::registerClassFactory(m_pSystem);
}

void ObjectSystemTest::Object_Instansing() {
    ATestObject obj1;

    AObject *result = m_pSystem->objectCreate(ATestObject::typeNameS());
    AObject *object = dynamic_cast<AObject*>(&obj1);

    QCOMPARE((result != 0), true);
    QCOMPARE((object != 0), true);
    QCOMPARE((*object == *result), true);

    delete result;
}

void ObjectSystemTest::Serialize_Desirialize_Object() {
    ATestObject obj1;
    ATestObject obj2;
    ATestObject obj3;
    obj1.setName("MainObject");
    obj1.addComponent("TestComponent2", &obj2);
    obj1.addComponent("TestComponent3", &obj3);
    AObject::addEventListner(&obj1, TSIGNAL, &obj2, TSLOT);
    AObject::addEventListner(&obj1, TSIGNAL, &obj3, TSLOT);
    AObject::addEventListner(&obj2, TSIGNAL, &obj3, TSLOT);
    AObject::addEventListner(&obj1, TPROPERTY1, &obj2, TPROPERTY1);
    AObject::addEventListner(&obj1, TPROPERTY1, &obj3, TPROPERTY1);
    AObject::addEventListner(&obj2, TPROPERTY1, &obj3, TPROPERTY1);

    AVariant data   = obj1.toVariant();
    AObject *result = AObject::toObject(data, m_pSystem);
    AObject *object = dynamic_cast<AObject*>(&obj1);

    QCOMPARE((result != 0), true);
    QCOMPARE((object != 0), true);
    QCOMPARE((*object == *result), true);

    delete result;
}

void ObjectSystemTest::Delta_Serialize_Desirialize_Object() {
    AObject *instance   = m_pSystem->objectCreate(ATestObject::typeNameS());
    AObject *obj1       = m_pSystem->objectCreate(ATestObject::typeNameS());
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

    delete instance;
    delete result;
}

void ObjectSystemTest::Virtual_Inheritance() {
    AObject *prototype  = m_pSystem->objectCreate(ATestObject::typeNameS());
    prototype->setSystem(m_pSystem);
    prototype->setName("PrototypeObject");

    AObject *obj1       = m_pSystem->objectCreate(ATestObject::typeNameS());
    obj1->setSystem(m_pSystem);
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
}
