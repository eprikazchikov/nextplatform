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

#ifndef MATRIX4_H_HEADER_INCLUDED
#define MATRIX4_H_HEADER_INCLUDED

#include "acommon.h"

class AVector3D;
class AVector4D;
class AMatrix3D;

class NEXT_LIBRARY_EXPORT AMatrix4D {
public:
    AMatrix4D                   ();
    AMatrix4D                   (const AMatrix3D &m);

    inline bool                 operator==                  (const AMatrix4D &v) const;
    inline bool                 operator!=                  (const AMatrix4D &v) const;

    inline const AVector3D      operator*                   (const AVector3D &v) const;
    inline const AVector4D      operator*                   (const AVector4D &v) const;
    inline const AMatrix4D      operator*                   (areal f) const;
    inline const AMatrix4D      operator*                   (const AMatrix4D &m) const;
    inline const AMatrix4D      operator+                   (const AMatrix4D &m) const;
    inline const AMatrix4D      operator-                   (const AMatrix4D &m) const;

    AMatrix4D                  &operator*=                  (areal f);
    AMatrix4D                  &operator*=                  (const AMatrix4D &m);
    AMatrix4D                  &operator+=                  (const AMatrix4D &m);
    AMatrix4D                  &operator-=                  (const AMatrix4D &m);

    areal                      &operator[]                  (int i);
    const areal                 operator[]                  (int i) const;
	
    AMatrix3D                   rotation                    () const;
    AMatrix4D                   transpose                   () const;
    areal                       determinant                 () const;
    AMatrix4D                   inverse                     () const;
    void                        reflect                     (const AVector4D &plane);
    void                        direction                   (const AVector3D &dir, AVector3D &up);

    void                        zero                        ();
    void                        identity                    ();
    void                        rotate                      (const AVector3D &axis, areal angle);
    void                        rotate                      (const AVector3D &angles);
    void                        scale                       (const AVector3D &v);
    void                        translate                   (const AVector3D &v);

    void                        perspective                 (areal fov, areal aspect, areal znear, areal zfar);
    void                        ortho                       (areal left, areal right, areal bottom, areal top, areal znear, areal zfar);
    void                        lookAt                      (AVector3D &eye, AVector3D &target, AVector3D &up);

    AVector3D                   euler                       ();

    areal                       mat[16];
};

#endif /* MATRIX4_H_HEADER_INCLUDED */
