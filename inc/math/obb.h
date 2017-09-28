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

    Copyright: 2008-2017 Evgeny Prikazchikov
*/

#ifndef OBB_H_HEADER_INCLUDED
#define OBB_H_HEADER_INCLUDED

#include "acommon.h"

#include "vector3.h"
#include "matrix3.h"

class AMatrix4D;

class NEXT_LIBRARY_EXPORT AOBox {
public:
    AOBox                       ();
    AOBox                       (const AVector3D &p, const AVector3D &s);
    AOBox                       (const AVector3D &p, const AVector3D &s, const AMatrix3D &r);

    inline const AOBox          operator*                   (areal f);
    inline const AOBox          operator*                   (const AVector3D &v);

    void                        box                         (AVector3D &min, AVector3D &max) const;
    void                        setBox                      (const AVector3D &min, const AVector3D &max);

    AVector3D                   pos;
    AVector3D                   size;
    AMatrix3D                   rot;
    areal                       radius;
};

#endif // OBB_H_HEADER_INCLUDED
