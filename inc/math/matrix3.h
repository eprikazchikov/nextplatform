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

#ifndef MATRIX3_H_HEADER_INCLUDED
#define MATRIX3_H_HEADER_INCLUDED

#include "acommon.h"

class AVector3D;
class AVector4D;

class NEXT_LIBRARY_EXPORT AMatrix3D {
public:
    AMatrix3D                   ();
    AMatrix3D                   (const AMatrix3D &m);

    inline bool                 operator==                  (const AMatrix3D &v) const;
    inline bool                 operator!=                  (const AMatrix3D &v) const;

    inline const AVector3D      operator*                   (const AVector3D &v) const;
    inline const AVector4D      operator*                   (const AVector4D &v) const;
    inline const AMatrix3D      operator*                   (areal f) const;
    inline const AMatrix3D      operator*                   (const AMatrix3D &m) const;
    inline const AMatrix3D      operator+                   (const AMatrix3D &m) const;
    inline const AMatrix3D      operator-                   (const AMatrix3D &m) const;
	
    AMatrix3D                  &operator*=                  (areal f);
    AMatrix3D                  &operator*=                  (const AMatrix3D &m);
    AMatrix3D                  &operator+=                  (const AMatrix3D &m);
    AMatrix3D                  &operator-=                  (const AMatrix3D &m);
	
    areal                      &operator[]                  (int i);
    const areal                 operator[]                  (int i) const;
	
    AMatrix3D                   transpose                   () const;
    areal                       determinant                 () const;
    AMatrix3D                   inverse                     () const;
	
    void                        zero                        ();
    void                        identity                    ();
    void                        rotate                      (const AVector3D &axis, areal angle);
    void                        rotate                      (const AVector3D &angles);
    void                        scale                       (const AVector3D &v);

    void                        orthonormalize              ();

    AVector3D                   euler                       ();

    areal                       mat[9];
};

#endif /* MATRIX3_H_HEADER_INCLUDED */
