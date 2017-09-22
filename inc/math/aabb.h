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

#ifndef AABB_H_HEADER_INCLUDED
#define AABB_H_HEADER_INCLUDED

#include "acommon.h"

#include "vector3.h"

class AMatrix4D;

class NEXT_LIBRARY_EXPORT AABox {
public:
    AABox                       ();
    AABox                       (const AVector3D &p, const AVector3D &s);

    inline const AABox          operator*                   (areal f) const;
    inline const AABox          operator*                   (const AVector3D &v) const;
    inline const AABox          operator*                   (const AMatrix4D &m) const;

    bool                        intersect                   (const AVector3D &p, areal r) const;

    void                        box                         (AVector3D &min, AVector3D &max) const;
    void                        setBox                      (const AVector3D &min, const AVector3D &max);

    AVector3D                   pos;
    AVector3D                   size;
};

#endif /* AABB_H_HEADER_INCLUDED */
