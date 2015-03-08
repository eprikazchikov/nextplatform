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

#ifndef TST_OBJECT_H
#define TST_OBJECT_H

#include <QObject>

class ObjectTest : public QObject {
    Q_OBJECT

private slots:
    void Base_add_remove_link();
    void Child_destructor();
    void Reciever_destructor();
    void Sender_destructor();
    void Emit_signal();
    void Synchronize_property();
    void Find_object();
    void Serialize_Desirialize_Object();
};

#endif // TST_OBJECT_H
