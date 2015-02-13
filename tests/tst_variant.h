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

#ifndef TST_VARIANT_H
#define TST_VARIANT_H

#include <QObject>

class VariantTest : public QObject {
    Q_OBJECT

private slots:
    void Set_Get_check();
    void Value_to_Shared();
    void Shared_to_Value();
    void Convert_Bool_to_Int_Float_String();
    void Convert_Int_to_Bool_Float_String();
    void Convert_Float_to_Bool_Int_String();
    void Convert_String_to_Bool_Int_Float();
};

#endif // TST_VARIANT_H
