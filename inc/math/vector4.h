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

#ifndef VECTOR4_H_HEADER_INCLUDED
#define VECTOR4_H_HEADER_INCLUDED

#include "acommon.h"

class AVector2D;
class AVector3D;

class NEXT_LIBRARY_EXPORT AVector4D {
public:
    AVector4D                   ();
    AVector4D                   (areal v);
    AVector4D                   (areal x, areal y, areal z, areal w);
    AVector4D                   (const AVector2D &v, areal z, areal w);
    AVector4D                   (const AVector3D &v, areal w);

    inline bool                 operator==                  (const AVector4D &vector) const;
    inline bool                 operator!=                  (const AVector4D &vector) const;
    inline bool                 operator>                   (const AVector4D &vector) const;
    inline bool                 operator<                   (const AVector4D &vector) const;

    inline const AVector4D      operator*                   (areal factor) const;
    inline const AVector4D      operator*                   (const AVector4D &vector) const;
    inline const AVector4D      operator/                   (areal divisor) const;
    inline const AVector4D      operator+                   (const AVector4D &vector) const;
    inline const AVector4D      operator-                   () const;
    inline const AVector4D      operator-                   (const AVector4D &vector) const;

    AVector4D                  &operator*=                  (areal factor);
    AVector4D                  &operator/=                  (areal divisor);
    AVector4D                  &operator+=                  (const AVector4D &vector);
    AVector4D                  &operator-=                  (const AVector4D &vector);

    areal                      &operator[]                  (int i);
    const areal                 operator[]                  (int i) const;

    areal                       length                      () const;
    areal                       sqrLength                   () const;

    areal                       normalize                   ();

    areal                       dot                         (const AVector4D &vector) const;

    union {
        struct {
            areal x, y, z, w;
        };
        areal v[4];
    };
};

#endif /* VECTOR4_H_HEADER_INCLUDED */
