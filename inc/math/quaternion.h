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

#ifndef QUATERNION_H_HEADER_INCLUDED
#define QUATERNION_H_HEADER_INCLUDED

#include "acommon.h"

class AVector3D;
class AMatrix3D;

class NEXT_LIBRARY_EXPORT AQuaternion {
public:
    AQuaternion                 ();
    AQuaternion                 (const AVector3D &dir, areal angle);
    AQuaternion                 (const AVector3D &euler);
    AQuaternion                 (const AMatrix3D &matrix);

    inline bool                 operator==                  (const AQuaternion &quaternion) const;
    inline bool                 operator!=                  (const AQuaternion &quaternion) const;
	
    inline AQuaternion          operator*                   (const AQuaternion &q) const;
    inline AVector3D            operator*                   (const AVector3D &v) const;

    areal                      &operator[]                  (int i);
    const areal                 operator[]                  (int i) const;
	
    AQuaternion                 inverse                     () const;

    void                        mix                         (const AQuaternion &q0, const AQuaternion &q1, areal t);
	
    AMatrix3D                   toMatrix                    () const;
    AVector3D                   euler                       () const;
	
    union {
        struct {
            areal x, y, z, w;
        };
        areal q[4];
    };
};

#endif /* QUATERNION_H_HEADER_INCLUDED */
