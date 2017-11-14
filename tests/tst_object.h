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

    Copyright: 2008-2016 Evgeniy Prikazchikov
*/

#ifndef TST_OBJECT_H
#define TST_OBJECT_H

#include <QObject>

class ObjectTest : public QObject {
    Q_OBJECT

private slots:
    void Meta_type();
    void Meta_property();
    void Meta_methods();
    void Disconnect_base();
    void Disconnect_all();
    void Disconnect_by_signal();
    void Disconnect_by_receiver();
    void Child_destructor();
    void Reciever_destructor();
    void Emit_signal();
    void Find_object();
    void Clone_object();
};

#endif // TST_OBJECT_H
