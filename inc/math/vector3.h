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

#ifndef VECTOR3_H_HEADER_INCLUDED
#define VECTOR3_H_HEADER_INCLUDED

#include "acommon.h"

class AVector2D;

class NEXT_LIBRARY_EXPORT AVector3D {
public:
    AVector3D                   ();
    AVector3D                   (areal v);
    AVector3D                   (areal x, areal y, areal z);
    AVector3D                   (const AVector2D &v, areal z);
    AVector3D                   (const areal *v);

    inline bool                 operator==                  (const AVector3D &vector) const;
    inline bool                 operator!=                  (const AVector3D &vector) const;
    inline bool                 operator>                   (const AVector3D &vector) const;
    inline bool                 operator<                   (const AVector3D &vector) const;

    inline const AVector3D      operator*                   (areal factor) const;
    inline const AVector3D      operator*                   (const AVector3D &vector) const;
    inline const AVector3D      operator/                   (areal divisor) const;
    inline const AVector3D      operator+                   (const AVector3D &vector) const;
    inline const AVector3D      operator-                   () const;
    inline const AVector3D      operator-                   (const AVector3D &vector) const;

    AVector3D                  &operator*=                  (areal factor);
    AVector3D                  &operator/=                  (areal divisor);
    AVector3D                  &operator+=                  (const AVector3D &vector);
    AVector3D                  &operator-=                  (const AVector3D &vector);
    
    areal                      &operator[]                  (int i);
    const areal                 operator[]                  (int i) const;
    
    areal                       length                      () const;
    areal                       sqrLength                   () const;

    areal                       normalize                   ();

    AVector3D                   cross                       (const AVector3D &vector) const;
    areal                       dot                         (const AVector3D &vector) const;

    union {
        struct {
            areal x, y, z;
        };
        areal v[3];
    };
};

#endif /* VECTOR3_H_HEADER_INCLUDED */
