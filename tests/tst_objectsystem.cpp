#include "tst_objectsystem.h"

#include "tst_common.h"

#include "aobjectsystem.h"

#include <QtTest>

void ObjectSystemTest::Serialize_Desirialize_Object() {
    ATestObject::registerClassFactory();

    ATestObject object;
    object.setComponent("TestComponent", 0);

    string data = AObjectSystem::instance()->save(object);

    qDebug() << data.c_str();
}
