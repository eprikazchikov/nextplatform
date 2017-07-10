#include "tst_objectsystem.h"

#include "tst_common.h"

#include "aobjectsystem.h"

#include "ajson.h"

#include <QtTest>

const string gTestPath  = "Test/Asset/Path";
ATestObject *resource;

void ObjectSystemTest::initTestCase() {
    resource   = AObjectSystem::objectCreate<ATestObject>();
    resource->setName("TestResource");
    //AObjectSystem::instance()->m_ResourceCache[gTestPath]   = resource;
}

void ObjectSystemTest::cleanupTestCase() {
    //auto it = AObjectSystem::instance()->m_ResourceCache.find(gTestPath);
    //if(it != AObjectSystem::instance()->m_ResourceCache.end()) {
    //    AObjectSystem::instance()->m_ResourceCache.erase(it);
    //}

    delete resource;
}

void ObjectSystemTest::Object_Instansing() {
    ATestObject obj1;

    AObject *result1    = AObjectSystem::objectCreate<ATestObject>();
    AObject *object = dynamic_cast<AObject*>(&obj1);

    QCOMPARE((result1 != 0), true);
    QCOMPARE((object != 0), true);
    QCOMPARE(compare(*object, *result1), true);

    AObject *result2    = AObjectSystem::objectCreate<ATestObject>();

    QCOMPARE((result1->uuid() != result2->uuid()), true);

    delete result1;
    delete result2;
}

void ObjectSystemTest::Serialize_Desirialize_Object() {
    ATestObject *obj1   = AObjectSystem::objectCreate<ATestObject>();
    ATestObject *obj2   = AObjectSystem::objectCreate<ATestObject>();
    ATestObject *obj3   = AObjectSystem::objectCreate<ATestObject>();

    obj1->setName("MainObject");

    obj2->setName("TestComponent2");
    obj3->setName("TestComponent3");
    obj2->setParent(obj1);
    obj3->setParent(obj2);

    //obj1->setResource(resource);
    //obj1->setProperty("DynamicProperty", AVariant::fromValue(resource));

    AObject::connect(obj1, _SIGNAL(signal(bool)), obj2, _SLOT(setSlot(bool)));
    AObject::connect(obj1, _SIGNAL(signal(bool)), obj3, _SIGNAL(signal(bool)));
    AObject::connect(obj2, _SIGNAL(signal(bool)), obj3, _SLOT(setSlot(bool)));

    string data     = AJson::save(AObjectSystem::toVariant(obj1), 0);
    AObject *result = AObjectSystem::toObject(AJson::load(data));
    AObject *object = dynamic_cast<AObject*>(obj1);

    QCOMPARE((result != nullptr), true);
    QCOMPARE((object != nullptr), true);
    QCOMPARE(compare(*object, *result), true);

    delete result;

    delete obj3;
    delete obj2;
    delete obj1;
}

class ASecondObject : public ATestObject {
    A_REGISTER(ASecondObject, ATestObject, Test)

    A_NOMETHODS()
    A_NOPROPERTIES()

};

void ObjectSystemTest::RegisterUnregister_Object() {
    QCOMPARE((int)AObjectSystem::instance()->factories().size(), 1);
    ASecondObject::registerClassFactory();
    QCOMPARE((int)AObjectSystem::instance()->factories().size(), 2);
    ASecondObject::unregisterClassFactory();
    QCOMPARE((int)AObjectSystem::instance()->factories().size(), 1);
}

class ATestObjectEx : public ATestObject {
    A_OVERRIDE(ATestObjectEx, ATestObject, Test)

    A_NOMETHODS()
    A_NOPROPERTIES()

};

void ObjectSystemTest::Override_Object() {
    ATestObjectEx::registerClassFactory();

    AObject *object = AObjectSystem::objectCreate<ATestObject>();

    QCOMPARE((object != nullptr), true);
    const AMetaObject *meta = object->metaObject();

    QCOMPARE((dynamic_cast<ATestObjectEx *>(object) != nullptr), true);
    QCOMPARE(meta->methodCount(), 2);
    QCOMPARE(meta->propertyCount(), 3);

    int index   = meta->indexOfProperty("slot");
    QCOMPARE((index > -1), true);
    delete object;

    ATestObjectEx::unregisterClassFactory();

    object = AObjectSystem::objectCreate<ATestObject>();
    QCOMPARE((dynamic_cast<ATestObject *>(object) != nullptr), true);
    QCOMPARE((dynamic_cast<ATestObjectEx *>(object) == nullptr), true);
    delete object;
}
