#include "tst_objectsystem.h"

#include "tst_common.h"

#include "aobjectsystem.h"

#include "ajson.h"

#include <QtTest>

void ObjectSystemTest::initTestCase() {
    ATestObject::registerClassFactory();
}

void ObjectSystemTest::Object_Instansing() {
    ATestObject obj1;

    AObject *result = AObjectSystem::instance()->createObject(ATestObject::typeNameS());

    QCOMPARE((result != 0), true);
    QCOMPARE(*dynamic_cast<AObject*>(&obj1), *result);

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
    AObject *result = AObject::toObject(data);

    QCOMPARE((result != 0), true);
    QCOMPARE(*dynamic_cast<AObject*>(&obj1), *result);

    delete result;
}

void ObjectSystemTest::Delta_Serialize_Desirialize_Object() {
    ATestObject prototype;
    prototype.setName("PrototypeObject");

    ATestObject obj1;
    prototype.addComponent("TestComponent1", &obj1);
    AObject::addEventListner(&prototype, TSIGNAL, &obj1, TSLOT);
    AObject::addEventListner(&prototype, TPROPERTY1, &obj1, TPROPERTY1);

    AObject *instance   = AObjectSystem::instance()->createObject(ATestObject::typeNameS());
    AObject *obj3       = AObjectSystem::instance()->createObject(ATestObject::typeNameS());
    instance->addComponent("TestComponent2", obj3);
    AObject::addEventListner(instance, TSIGNAL, obj3, TSLOT);
    AObject::addEventListner(instance, TPROPERTY1, obj3, TPROPERTY1);

    instance->setName("CopiedObject");
    instance->setProperty(TPROPERTY1, true);
    instance->setProperty("Dynamic", 5);

    AVariant data   = instance->toVariant();
    qDebug() << AJson::save(data, 0).c_str();
    AObject *result = AObject::toObject(data);

    QCOMPARE((result != 0), true);
    QCOMPARE(prototype.property(TPROPERTY1).toBool(), false);
    QCOMPARE(result->property(TPROPERTY1).toBool(), true);
    QCOMPARE(*instance, *result);

    delete instance;
    delete result;
}
