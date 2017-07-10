/*
    This file is part of Thunder Next.

    Thunder Next is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Thunder Next is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Thunder Next.  If not, see <http://www.gnu.org/licenses/>.

    Copyright: 2008-2014 Evgeniy Prikazchikov
*/

#include "tst_object.h"

#include "tst_common.h"

#include "auri.h"

#include <QtTest>

bool toList(void *to, const void *from, const uint32_t fromType) {
    if(fromType == AMetaType::type<ATestObject *>()) {
        const AObject *o  = *(const AObject **)from;

        AVariant::AVariantList *r    = static_cast<AVariant::AVariantList *>(to);
        *r  = AObjectSystem::toVariant(o).value<AVariant::AVariantList>();

        return true;
    }
    return false;
}

void ObjectTest::Meta_type() {
    int type    = AMetaType::type<ATestObject *>();
    bool result = AMetaType::registerConverter(type, AMetaType::VariantList, &toList);

    QCOMPARE(result, true);

    ATestObject *obj    = new ATestObject;
    AVariant variant    = AVariant::fromValue(obj);

    QCOMPARE(variant.isValid(), true);
    QCOMPARE((int)variant.userType(), type);

    AVariant::AVariantList list = variant.toList();
    QCOMPARE((int)list.size(), 2);
}

void ObjectTest::Meta_property() {
    ATestObject obj;
    const AMetaObject *meta = obj.metaObject();
    QVERIFY(meta != nullptr);

    QCOMPARE(meta->name(), "ATestObject");

    QCOMPARE(meta->propertyCount(), 3);
    QCOMPARE(meta->property(0).isValid(), true);
    QCOMPARE(meta->property(1).isValid(), true);

    QCOMPARE(meta->property(0).read(&obj).toBool(), obj.getSlot());
    obj.setSlot(true);
    QCOMPARE(meta->property(0).read(&obj).toBool(), obj.getSlot());

    {
        bool value  = false;
        meta->property(0).write(&obj, value);
        QCOMPARE(obj.getSlot(), value);
    }
    {
        AVector2D value(1.0, 2.0);
        meta->property(1).write(&obj, value);
        QCOMPARE(obj.getVector().x, value.x);
        QCOMPARE(obj.getVector().y, value.y);
    }
}

void ObjectTest::Meta_methods() {
    ATestObject obj;
    const AMetaObject *meta = obj.metaObject();
    QVERIFY(meta != nullptr);

    QCOMPARE(meta->methodCount(), 2);

    int index   = meta->indexOfMethod("setSlot");
    if(index > -1) {
        AMetaMethod method = meta->method(index);

        QCOMPARE(method.isValid(), true);
        AVariant value;
        QCOMPARE(obj.getSlot(), false);

        AVariant arg(true);
        QCOMPARE(method.invoke(&obj, value, 1, &arg), true);
        QCOMPARE(obj.getSlot(), true);
    }

    QCOMPARE(meta->indexOfSignal("setSlot"), -1);

    index   = meta->indexOfSignal("signal");
    if(index > -1) {
        qDebug() << meta->method(index).signature().c_str();
    }
}

void ObjectTest::Disconnect_base() {
    ATestObject obj1;
    ATestObject obj2;
    ATestObject obj3;

    AObject::connect(&obj1, _SIGNAL(signal(bool)), &obj2, _SLOT(setSlot(bool)));
    AObject::connect(&obj1, _SIGNAL(signal(bool)), &obj3, _SIGNAL(signal(bool)));
    QCOMPARE((int)obj1.getReceivers().size(), 2);

    AObject::disconnect(&obj1, _SIGNAL(signal(bool)), &obj3, _SIGNAL(signal(bool)));
    QCOMPARE((int)obj1.getReceivers().size(), 1);
}

void ObjectTest::Disconnect_all() {
    ATestObject obj1;
    ATestObject obj2;
    ATestObject obj3;

    AObject::connect(&obj1, _SIGNAL(signal(bool)), &obj2, _SLOT(setSlot(bool)));
    AObject::connect(&obj1, _SIGNAL(signal(bool)), &obj3, _SIGNAL(signal(bool)));
    QCOMPARE((int)obj1.getReceivers().size(), 2);
    QCOMPARE((int)obj3.getSenders().size(), 1);

    AObject::disconnect(&obj1, 0, 0, 0);
    QCOMPARE((int)obj1.getReceivers().size(), 0);
    QCOMPARE((int)obj2.getSenders().size(), 0);
    QCOMPARE((int)obj3.getSenders().size(), 0);
}

void ObjectTest::Disconnect_by_signal() {
    ATestObject obj1;
    ATestObject obj2;
    ATestObject obj3;

    AObject::connect(&obj1, _SIGNAL(signal(bool)), &obj2, _SLOT(setSlot(bool)));
    AObject::connect(&obj1, _SIGNAL(signal(bool)), &obj3, _SIGNAL(signal(bool)));
    QCOMPARE((int)obj1.getReceivers().size(), 2);
    QCOMPARE((int)obj3.getSenders().size(), 1);

    AObject::disconnect(&obj1, _SIGNAL(signal(bool)), 0, 0);
    QCOMPARE((int)obj1.getReceivers().size(), 0);
    QCOMPARE((int)obj2.getSenders().size(), 0);
    QCOMPARE((int)obj3.getSenders().size(), 0);
}

void ObjectTest::Disconnect_by_receiver() {
    ATestObject obj1;
    ATestObject obj2;
    ATestObject obj3;

    AObject::connect(&obj1, _SIGNAL(signal(bool)), &obj2, _SLOT(setSlot(bool)));
    AObject::connect(&obj1, _SIGNAL(signal(bool)), &obj3, _SIGNAL(signal(bool)));
    QCOMPARE((int)obj1.getReceivers().size(), 2);
    QCOMPARE((int)obj3.getSenders().size(), 1);

    AObject::disconnect(&obj1, 0, &obj3, 0);
    QCOMPARE((int)obj1.getReceivers().size(), 1);
    QCOMPARE((int)obj3.getSenders().size(), 0);
}

void ObjectTest::Child_destructor() {
    AObject *obj1   = new AObject;
    AObject *obj2   = new AObject();
    obj2->setName("TestComponent2");
    obj2->setParent(obj1);

    AObject *obj3   = new AObject();
    obj3->setName("TestComponent3");
    obj3->setParent(obj1);

    QCOMPARE((int)obj1->getChildren().size(), 2);

    delete obj2;
    QCOMPARE((int)obj1->getChildren().size(), 1);

    obj3->deleteLater();
    obj3->processEvents();
    QCOMPARE((int)obj1->getChildren().size(), 0);

    delete obj1;
}

void ObjectTest::Reciever_destructor() {
    ATestObject *obj1   = new ATestObject;
    ATestObject *obj2   = new ATestObject;

    AObject::connect(obj1, _SIGNAL(signal(bool)), obj2, _SIGNAL(signal(bool)));
    QCOMPARE((int)obj1->getReceivers().size(),  1);
    QCOMPARE((int)obj2->getSenders().size(),  1);

    delete obj2;
    QCOMPARE((int)obj1->getReceivers().size(),  0);

    delete obj1;
}

void ObjectTest::Sender_destructor() {
    ATestObject *obj1   = new ATestObject;
    ATestObject *obj2   = new ATestObject;

    AObject::connect(obj1, _SIGNAL(signal(bool)), obj2, _SIGNAL(signal(bool)));
    QCOMPARE((int)obj1->getReceivers().size(),  1);
    QCOMPARE((int)obj2->getSenders().size(),  1);

    delete obj1;
    QCOMPARE((int)obj2->getSenders().size(),  0);

    delete obj2;
}

void ObjectTest::Emit_signal() {
    ATestObject obj1;
    ATestObject obj2;
    ATestObject obj3;

    AObject::connect(&obj1, _SIGNAL(signal(bool)), &obj2, _SIGNAL(signal(bool)));
    AObject::connect(&obj2, _SIGNAL(signal(bool)), &obj3, _SLOT(setSlot(bool)));

    {
        QCOMPARE(obj3.m_bSlot, false);
        obj2.emitSignal(_SIGNAL(signal(bool)), true);
        obj1.processEvents();
        obj2.processEvents();
        obj3.processEvents();

        QCOMPARE(obj3.m_bSlot, true);
    }
    {
        QCOMPARE(obj3.m_bSlot, true);
        obj1.emitSignal(_SIGNAL(signal(bool)), false);
        obj1.processEvents();
        obj2.processEvents();
        obj3.processEvents();

        QCOMPARE(obj3.m_bSlot, false);
    }
}

void ObjectTest::Find_object() {
    AObject obj1;
    ATestObject obj2;
    ATestObject obj3;

    obj1.setName("MainObject");
    obj2.setName("TestComponent2");
    obj3.setName("TestComponent3");
    obj2.setParent(&obj1);
    obj3.setParent(&obj1);
    {
        AObject *result = obj1.find("/MainObject/TestComponent2");
        QCOMPARE(result, &obj2);
    }
    {
        AObject *result = obj1.find("/MainObject/TestComponent3");
        QCOMPARE(&obj3, result);
    }
    {
        ATestObject *result = obj1.findChild<ATestObject *>();
        QCOMPARE(&obj2, result);
    }
    {
        list<ATestObject *> result = obj1.findChildren<ATestObject *>();
        QCOMPARE(int(result.size()), 2);
    }
}

void ObjectTest::Clone_object() {
    ATestObject obj1;
    ATestObject obj2;
    obj1.setName("MainObject");
    obj2.setName("TestComponent2");
    obj2.setParent(&obj1);
    obj1.setVector(AVector2D(10.0, 20.0));
    obj1.setProperty("dynamic1", 100);
    obj2.setProperty("dynamic2", true);

    AObject::connect(&obj1, _SIGNAL(signal(bool)), &obj2, _SLOT(setSlot(bool)));
    AObject::connect(&obj2, _SIGNAL(signal(bool)), &obj1, _SLOT(setSlot(bool)));

    AObject *clone  = obj1.clone();
    QCOMPARE((clone != nullptr), true);
    QCOMPARE(compare(*clone, obj1), true);
    delete clone;
}
