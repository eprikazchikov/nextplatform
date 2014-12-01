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

    © Copyright: 2008-2014 Evgeny Prikazchikov
*/

#ifndef OBB_H_HEADER_INCLUDED
#define OBB_H_HEADER_INCLUDED

struct AOBox {
    AOBox() : pos(0), size(1)                                                                   { }
    AOBox(const AVector3D &p, const AVector3D &s) : pos(p), size(s)                             { }
    AOBox(const AVector3D &p, const AVector3D &s, const AMatrix3D &r) : pos(p), size(s), rot(r) { }

    const AOBox operator*(float f) {
        AVector3D c     = pos * f;
        AVector3D s     = size * f;
        return AOBox(c, s, rot);
    }

    const AOBox operator*(const AVector3D &v) {
        AVector3D p(pos * v);
        AVector3D s(size * v);
        return AOBox(p, s, rot);
    }

    void set_box(AVector3D &min, AVector3D &max) {
        size    = max - min;
        pos     = min + size * 0.5;
    }

    void get_box(AVector3D &min, AVector3D &max) {
        min     = pos - size * 0.5;
        max     = min + size;
    }

    void get_box(AVector3D *b, const AMatrix4D &m) {
        AVector3D min, max;
        get_box(min, max);

        AVector3D t(m[12], m[13], m[14]);

        AMatrix3D r;
        r[0] = m[0]; r[3] = m[4]; r[6] = m[8];
        r[1] = m[1]; r[4] = m[5]; r[7] = m[9];
        r[2] = m[2]; r[5] = m[6]; r[8] = m[10];

        b[0]  = r * AVector3D(min.x, min.y, min.z) + t;
        b[1]  = r * AVector3D(min.x, min.y, max.z) + t;
        b[2]  = r * AVector3D(max.x, min.y, max.z) + t;
        b[3]  = r * AVector3D(max.x, min.y, min.z) + t;
        b[4]  = r * AVector3D(min.x, max.y, min.z) + t;
        b[5]  = r * AVector3D(min.x, max.y, max.z) + t;
        b[6]  = r * AVector3D(max.x, max.y, max.z) + t;
        b[7]  = r * AVector3D(max.x, max.y, min.z) + t;
    }

    AVector3D pos;      // Delta position from AOBox center
    AVector3D size;     // Size of AOBox
    AMatrix3D rot;      // Rotation transform matrix;
};

#endif // OBB_H_HEADER_INCLUDED
