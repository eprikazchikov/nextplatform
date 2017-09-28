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

#ifndef PLANE_H_HEADER_INCLUDED
#define PLANE_H_HEADER_INCLUDED

#include "acommon.h"

#include "vector3.h"

class NEXT_LIBRARY_EXPORT APlane {
public:
    APlane                      ();
    APlane                      (const AVector3D &v1, const AVector3D &v2, const AVector3D &v3);

    areal                       distance                    (const AVector3D &p);

    AVector3D                   normal;
    AVector3D                   point;
    float                       d;
};

#endif /* PLANE_H_HEADER_INCLUDED */
