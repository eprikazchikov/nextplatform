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

    � Copyright: 2008-2014 Evgeny Prikazchikov
*/

#include "tst_object.h"

#include "tst_common.h"

#include <QtTest>

void ObjectTest::Base_add_remove_link() {
    AObject *obj1   = new AObject(0);
    AObject *obj2   = new AObject(0);

    AObject::addEventListner(obj1, TSIGNAL, obj2, TSIGNAL);
    QCOMPARE((int)obj1->getLinks().size(), 1);

    AObject::removeEventListner(obj1, TSIGNAL, obj2, TSIGNAL);
    QCOMPARE((int)obj1->getLinks().size(), 0);

    delete obj2;
    delete obj1;
}

void ObjectTest::Reciever_destructor() {
    AObject *obj1   = new AObject(0);
    AObject *obj2   = new AObject(0);

    AObject::addEventListner(obj1, TSIGNAL, obj2, TSIGNAL);
    QCOMPARE((int)obj1->getLinks().size(), 1);

    delete obj2;
    QCOMPARE((int)obj1->getLinks().size(), 0);

    delete obj1;
}

void ObjectTest::Sender_destructor() {
    AObject *obj1   = new AObject(0);
    AObject *obj2   = new AObject(0);

    AObject::addEventListner(obj1, TSIGNAL, obj2, TSIGNAL);
    QCOMPARE((int)obj1->getLinks().size(), 1);

    delete obj1;
    QCOMPARE((int)obj2->getLinks().size(), 0);

    delete obj2;
}

void ObjectTest::Emit_signal() {
    ATestObject *obj1   = new ATestObject(0);
    ATestObject *obj2   = new ATestObject(0);

    AObject::addEventListner(obj1, TSIGNAL, obj2, TSLOT);
    obj1->emitSignal(TSIGNAL, AObject::variant_vector(1, TVALUE));

    QCOMPARE(obj2->m_bSlot, true);

    delete obj1;
    delete obj2;
}

void ObjectTest::Synchronize_property() {
    ATestObject *obj1   = new ATestObject(0);
    ATestObject *obj2   = new ATestObject(0);

    AObject::addEventListner(obj1, TPROPERTY, obj2, TPROPERTY);
    obj1->setProperty(TPROPERTY, true);

    QCOMPARE(obj1->property(TPROPERTY).toBool(), true);
    QCOMPARE(obj2->property(TPROPERTY).toBool(), true);

    delete obj1;
    delete obj2;
}
