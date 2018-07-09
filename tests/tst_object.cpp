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

#include "core/uri.h"

#include <QtTest>

bool toList(void *to, const void *from, const uint32_t fromType) {
    if(fromType == MetaType::type<TestObject *>()) {
        const Object *o = *(const Object **)from;

        VariantList *r  = static_cast<VariantList *>(to);
        *r  = ObjectSystem::toVariant(o).value<VariantList>();

        return true;
    }
    return false;
}

void ObjectTest::Meta_type() {
    int type    = MetaType::type<TestObject *>();
    bool result = MetaType::registerConverter(type, MetaType::VARIANTLIST, &toList);

    QCOMPARE(result, true);

    TestObject *obj = new TestObject;
    Variant variant = Variant::fromValue(obj);

    QCOMPARE(variant.isValid(), true);
    QCOMPARE((int)variant.userType(), type);

    VariantList list    = variant.toList();
    QCOMPARE((int)list.size(), 1);
}

void ObjectTest::Meta_property() {
    TestObject obj;
    const MetaObject *meta  = obj.metaObject();
    QVERIFY(meta != nullptr);

    QCOMPARE(meta->name(), "TestObject");

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
        Vector2 value(1.0, 2.0);
        meta->property(1).write(&obj, value);
        QCOMPARE(obj.getVector().x, value.x);
        QCOMPARE(obj.getVector().y, value.y);
    }
}

void ObjectTest::Meta_methods() {
    TestObject obj;
    const MetaObject *meta = obj.metaObject();
    QVERIFY(meta != nullptr);

    QCOMPARE(meta->methodCount(), 2);

    int index   = meta->indexOfMethod("setSlot");
    if(index > -1) {
        MetaMethod method   = meta->method(index);

        QCOMPARE(method.isValid(), true);
        Variant value;
        QCOMPARE(obj.getSlot(), false);

        Variant arg(true);
        QCOMPARE(method.invoke(&obj, value, 1, &arg), true);
        QCOMPARE(obj.getSlot(), true);
    }

    QCOMPARE(meta->indexOfSignal("setSlot"), -1);

    index   = meta->indexOfSignal("signal(bool)");
    if(index > -1) {
        //qDebug() << meta->method(index).signature().c_str();
    }
}

void ObjectTest::Disconnect_base() {
    TestObject obj1;
    TestObject obj2;
    TestObject obj3;

    Object::connect(&obj1, _SIGNAL(signal(bool)), &obj2, _SLOT(setSlot(bool)));
    Object::connect(&obj1, _SIGNAL(signal(bool)), &obj3, _SIGNAL(signal(bool)));
    QCOMPARE((int)obj1.getReceivers().size(), 2);

    Object::disconnect(&obj1, _SIGNAL(signal(bool)), &obj3, _SIGNAL(signal(bool)));
    QCOMPARE((int)obj1.getReceivers().size(), 1);
}

void ObjectTest::Disconnect_all() {
    TestObject obj1;
    TestObject obj2;
    TestObject obj3;

    Object::connect(&obj1, _SIGNAL(signal(bool)), &obj2, _SLOT(setSlot(bool)));
    Object::connect(&obj1, _SIGNAL(signal(bool)), &obj3, _SIGNAL(signal(bool)));
    QCOMPARE((int)obj1.getReceivers().size(), 2);

    Object::disconnect(&obj1, 0, 0, 0);
    QCOMPARE((int)obj1.getReceivers().size(), 0);
}

void ObjectTest::Disconnect_by_signal() {
    TestObject obj1;
    TestObject obj2;
    TestObject obj3;

    Object::connect(&obj1, _SIGNAL(signal(bool)), &obj2, _SLOT(setSlot(bool)));
    Object::connect(&obj1, _SIGNAL(signal(bool)), &obj3, _SIGNAL(signal(bool)));
    QCOMPARE((int)obj1.getReceivers().size(), 2);

    Object::disconnect(&obj1, _SIGNAL(signal(bool)), 0, 0);
    QCOMPARE((int)obj1.getReceivers().size(), 0);
}

void ObjectTest::Disconnect_by_receiver() {
    TestObject obj1;
    TestObject obj2;
    TestObject obj3;

    Object::connect(&obj1, _SIGNAL(signal(bool)), &obj2, _SLOT(setSlot(bool)));
    Object::connect(&obj1, _SIGNAL(signal(bool)), &obj3, _SIGNAL(signal(bool)));
    QCOMPARE((int)obj1.getReceivers().size(), 2);

    Object::disconnect(&obj1, 0, &obj3, 0);
    QCOMPARE((int)obj1.getReceivers().size(), 1);
}

void ObjectTest::Child_destructor() {
    Object *obj1   = new Object;
    Object *obj2   = new Object();
    obj2->setName("TestComponent2");
    obj2->setParent(obj1);

    Object *obj3   = new Object();
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
    TestObject *obj1    = new TestObject;
    TestObject *obj2    = new TestObject;

    Object::connect(obj1, _SIGNAL(signal(bool)), obj2, _SIGNAL(signal(bool)));
    QCOMPARE((int)obj1->getReceivers().size(),  1);

    delete obj2;
    QCOMPARE((int)obj1->getReceivers().size(),  0);

    delete obj1;
}

void ObjectTest::Emit_signal() {
    TestObject obj1;
    TestObject obj2;
    TestObject obj3;

    Object::connect(&obj1, _SIGNAL(signal(bool)), &obj2, _SIGNAL(signal(bool)));
    Object::connect(&obj2, _SIGNAL(signal(bool)), &obj3, _SLOT(setSlot(bool)));

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
    Object obj1;
    TestObject obj2;
    TestObject obj3;

    obj1.setName("MainObject");
    obj2.setName("TestComponent2");
    obj3.setName("TestComponent3");
    obj2.setParent(&obj1);
    obj3.setParent(&obj1);
    {
        Object *result  = obj1.find("/MainObject/TestComponent2");
        QCOMPARE(result, &obj2);
    }
    {
        Object *result  = obj1.find("/MainObject/TestComponent3");
        QCOMPARE(&obj3, result);
    }
    {
        TestObject *result  = obj1.findChild<TestObject *>();
        QCOMPARE(&obj2, result);
    }
    {
        list<TestObject *> result   = obj1.findChildren<TestObject *>();
        QCOMPARE(int(result.size()), 2);
    }
}

void ObjectTest::Clone_object() {
    TestObject obj1;
    TestObject obj2;
    obj1.setName("MainObject");
    obj2.setName("TestComponent2");
    obj2.setParent(&obj1);
    obj1.setVector(Vector2(10.0, 20.0));
    obj1.setProperty("dynamic1", 100);
    obj2.setProperty("dynamic2", true);

    Object::connect(&obj1, _SIGNAL(signal(bool)), &obj2, _SLOT(setSlot(bool)));
    Object::connect(&obj2, _SIGNAL(signal(bool)), &obj1, _SLOT(setSlot(bool)));

    Object *clone   = obj1.clone();
    QCOMPARE((clone != nullptr), true);
    QCOMPARE(compare(*clone, obj1), true);
    QCOMPARE((clone->uuid() != 0), true);
    delete clone;
}
