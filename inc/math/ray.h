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

#ifndef RAY_H_HEADER_INCLUDED
#define RAY_H_HEADER_INCLUDED

#include "acommon.h"

#include "vector3.h"

class APlane;
class AABox;

class NEXT_LIBRARY_EXPORT ARay {
public:
    ARay                        ();
    ARay                        (const AVector3D &p, const AVector3D &d);

    bool                        intersect                   (const AVector3D &p, areal r, AVector3D *pt);
    bool                        intersect                   (const APlane &p, AVector3D *pt, bool back = false);
    bool                        intersect                   (const AABox &b, AVector3D *pt);
    bool                        intersect                   (const AVector3D &v1, const AVector3D &v2, const AVector3D &v3, AVector3D *pt, bool back = false);

    ARay                        reflect                     (const AVector3D &n, const AVector3D &p);
    ARay                        refract                     (const AVector3D &n, const AVector3D &p, areal c0, areal c1);
    ARay                        diffuse                     (const AVector3D &n, const AVector3D &p, areal min, areal max);

    AVector3D                   pos;
    AVector3D                   dir;
};

#endif // RAY_H_HEADER_INCLUDED
