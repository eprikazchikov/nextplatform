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

    © Copyright: 2008-2014 Evgeny Prikazchikov
*/

#include "tst_object.h"

#include "tst_common.h"

#include "auri.h"

#include <QtTest>

void ObjectTest::Base_add_remove_link() {
    AObject *obj1   = new AObject();
    AObject *obj2   = new AObject();

    AObject::addEventListner(obj1, TSIGNAL, obj2, TSIGNAL);
    AObject::addEventListner(obj1, TSIGNAL, obj2, TSIGNAL);
    QCOMPARE((int)obj1->getLinks().size(), 1);

    AObject::removeEventListner(obj1, TSIGNAL, obj2, TSIGNAL);
    QCOMPARE((int)obj1->getLinks().size(), 0);

    delete obj2;
    delete obj1;
}

void ObjectTest::Child_destructor() {
    AObject *obj1   = new AObject();
    AObject *obj2   = new AObject();
    AObject *obj3   = new AObject();
    obj2->setParent(obj1);
    obj3->setParent(obj1);

    QCOMPARE((int)obj1->getComponents().size(), 2);

    delete obj2;
    QCOMPARE((int)obj1->getComponents().size(), 1);

    obj3->deleteLater();
    obj1->update(0);
    QCOMPARE((int)obj1->getComponents().size(), 0);

    delete obj1;
}

void ObjectTest::Reciever_destructor() {
    AObject *obj1   = new AObject();
    AObject *obj2   = new AObject();

    AObject::addEventListner(obj1, TSIGNAL, obj2, TSIGNAL);
    QCOMPARE((int)obj1->getLinks().size(), 1);

    delete obj2;
    QCOMPARE((int)obj1->getLinks().size(),      0);

    delete obj1;
}

void ObjectTest::Sender_destructor() {
    AObject *obj1   = new AObject();
    AObject *obj2   = new AObject();

    AObject::addEventListner(obj1, TSIGNAL, obj2, TSIGNAL);
    QCOMPARE((int)obj1->getLinks().size(), 1);

    delete obj1;
    QCOMPARE((int)obj2->getLinks().size(), 0);

    delete obj2;
}

void ObjectTest::Emit_signal() {
    ATestObject *obj1   = new ATestObject();
    ATestObject *obj2   = new ATestObject();

    AObject::addEventListner(obj1, TSIGNAL, obj2, TSLOT);
    obj1->emitSignal(TSIGNAL, TVALUE);

    QCOMPARE(obj2->m_bSlot, true);

    delete obj1;
    delete obj2;
}

void ObjectTest::Synchronize_property() {
    ATestObject *obj1   = new ATestObject();
    ATestObject *obj2   = new ATestObject();

    AObject::addEventListner(obj1, TPROPERTY1, obj2, TPROPERTY1);
    obj1->setProperty(TPROPERTY1, true);

    QCOMPARE(obj1->property(TPROPERTY1).isShared(), true);
    QCOMPARE(obj1->property(TPROPERTY1).toBool(), true);
    QCOMPARE(obj2->property(TPROPERTY1).isShared(), true);
    QCOMPARE(obj2->property(TPROPERTY1).toBool(), true);

    delete obj1;
    delete obj2;
}

void ObjectTest::Find_object() {
    ATestObject obj1;
    ATestObject obj2;
    ATestObject obj3;

    obj1.setName("Daddy");
    obj1.addComponent("TestComponent2", &obj2);
    obj1.addComponent("TestComponent3", &obj3);

    {
        AUri uri(obj2.reference());
        AObject *result = obj1.findObject(uri.path());

        QCOMPARE(&obj2, result);
    }

    {
        AUri uri(obj3.reference());
        AObject *result = obj1.findObject(uri.path());

        QCOMPARE(&obj3, result);
    }
}
