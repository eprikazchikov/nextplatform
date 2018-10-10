#include "tst_objectsystem.h"

#include "tst_common.h"

#include "objectsystem.h"

#include "json.h"
#include "bson.h"

#include <QtTest>

TestObject *resource;

void ObjectSystemTest::initTestCase() {
    ObjectSystem system;
    resource   = system.objectCreate<TestObject>();
    resource->setName("TestResource");
}

void ObjectSystemTest::cleanupTestCase() {
    delete resource;
}

void ObjectSystemTest::Object_Instansing() {
    TestObject obj1;
    ObjectSystem system;
    Object *result1 = system.objectCreate<TestObject>();
    Object *object  = dynamic_cast<Object*>(&obj1);

    QCOMPARE((result1 != 0), true);
    QCOMPARE((object != 0), true);
    QCOMPARE(compare(*object, *result1), true);

    Object *result2 = system.objectCreate<TestObject>();

    QCOMPARE((result1->uuid() != result2->uuid()), true);

    delete result1;
    delete result2;
}

void ObjectSystemTest::Serialize_Desirialize_Object() {
    ObjectSystem system;
    TestObject *obj1    = system.objectCreate<TestObject>();
    TestObject *obj2    = system.objectCreate<TestObject>();
    TestObject *obj3    = system.objectCreate<TestObject>();

    obj1->setName("MainObject");

    obj2->setName("TestComponent2");
    obj3->setName("TestComponent3");
    obj2->setParent(obj1);
    obj3->setParent(obj2);


    Object::connect(obj1, _SIGNAL(signal(bool)), obj2, _SLOT(setSlot(bool)));
    Object::connect(obj1, _SIGNAL(signal(bool)), obj3, _SIGNAL(signal(bool)));
    Object::connect(obj2, _SIGNAL(signal(bool)), obj3, _SLOT(setSlot(bool)));

    ByteArray bytes = Bson::save(system.toVariant(obj1));
    Object *result  = system.toObject(Bson::load(bytes));
    Object *object  = dynamic_cast<Object*>(obj1);

    QCOMPARE((result != nullptr), true);
    QCOMPARE((object != nullptr), true);
    QCOMPARE(compare(*object, *result), true);

    QCOMPARE((object->uuid() == result->uuid()), true);

    delete result;

    delete obj3;
    delete obj2;
    delete obj1;
}

class SecondObject : public TestObject {
    A_REGISTER(SecondObject, TestObject, Test)

    A_NOMETHODS()
    A_NOPROPERTIES()

};

void ObjectSystemTest::RegisterUnregister_Object() {
    ObjectSystem system;
    QCOMPARE((int)system.factories().size(), 1);
    SecondObject::registerClassFactory(&system);
    QCOMPARE((int)system.factories().size(), 2);
    SecondObject::unregisterClassFactory(&system);
    QCOMPARE((int)system.factories().size(), 1);
}

class TestObjectEx : public TestObject {
    A_OVERRIDE(TestObjectEx, TestObject, Test)

    A_NOMETHODS()
    A_NOPROPERTIES()
};

void ObjectSystemTest::Override_Object() {
    ObjectSystem system;
    TestObjectEx::registerClassFactory(&system);

    Object *object  = system.objectCreate<TestObject>();

    QCOMPARE((object != nullptr), true);
    const MetaObject *meta = object->metaObject();

    QCOMPARE((dynamic_cast<TestObjectEx *>(object) != nullptr), true);
    QCOMPARE(meta->methodCount(), 3);
    QCOMPARE(meta->propertyCount(), 3);

    int index   = meta->indexOfProperty("slot");
    QCOMPARE((index > -1), true);
    delete object;

    TestObjectEx::unregisterClassFactory(&system);

    object = system.objectCreate<TestObject>();
    QCOMPARE((dynamic_cast<TestObject *>(object) != nullptr), true);
    QCOMPARE((dynamic_cast<TestObjectEx *>(object) == nullptr), true);
    delete object;
}
