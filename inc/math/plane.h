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

class APlane {
public:
    APlane ()                                                               {}
    APlane (const AVector3D &v1, const AVector3D &v2, const AVector3D &v3)  {
        set_points(v1, v2, v3);
    }

    float distance(const AVector3D &p) {
        return normal.dot(p) - d;
    }

    void set_points(const AVector3D &v1, const AVector3D &v2, const AVector3D &v3) {
        AVector3D aux1, aux2;
        aux1    = v2 - v1;
        aux2    = v3 - v1;
        normal  = aux1.cross(aux2);
        //normal.normalize();
        point   = v1;
        d       = normal.dot(point);
    }

    AVector3D normal;
    AVector3D point;
    float d;
};

#endif /* PLANE_H_HEADER_INCLUDED */
