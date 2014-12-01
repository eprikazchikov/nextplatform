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

#ifndef RANGE3_H_HEADER_INCLUDED
#define RANGE3_H_HEADER_INCLUDED

struct ARange3D {
    inline ARange3D     () : mMin(0.0f), mMax(1.0f)                             { }
    inline ARange3D     (AVector3D min, AVector3D max) : mMin(min), mMax(max)   { }

    inline AVector3D    frand() {
        AVector3D v;
        v.x     = ((mMax.x - mMin.x)*((float)rand()/RAND_MAX)) + mMin.x;
        v.y     = ((mMax.y - mMin.y)*((float)rand()/RAND_MAX)) + mMin.y;
        v.z     = ((mMax.z - mMin.z)*((float)rand()/RAND_MAX)) + mMin.z;
        return v;
    }

    AVector3D       mMin;
    AVector3D       mMax;
};

#endif /* RANGE3_H_HEADER_INCLUDED */
