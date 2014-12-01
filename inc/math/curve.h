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

#ifndef CURVE_H_HEADER_INCLUDED
#define CURVE_H_HEADER_INCLUDED

struct ACurvePoint {
    inline ACurvePoint () {

    }

    inline ACurvePoint (float x, AVector3D &y, AVector3D &i, AVector3D &o) {
        mX  = x; mY = y; mI = i; mO = o;
    }

    float           mX;
    AVector3D       mY;
    AVector3D       mI;
    AVector3D       mO;
};

struct ACurve {
    typedef std::vector<ACurvePoint>    vector_point;

    inline ACurve () {
    }

    inline ACurve (ACurvePoint &p) {
        append (p);
    }

    inline ACurve (float x, AVector3D &y, AVector3D &in, AVector3D &out) {
        append (x, y, in, out);
    }

    inline void append (ACurvePoint &p) {
        pList.push_back(p);
    }

    inline void append (float x, AVector3D &y, AVector3D &in, AVector3D &out) {
        ACurvePoint p(x, y, in, out);
        append(p);
    }

    inline void insert (size_t index, ACurvePoint &p) {
        pList.insert(pList.begin() + index, p);
    }

    inline void insert (size_t index, float x, AVector3D &y, AVector3D &in, AVector3D &out) {
        ACurvePoint p(x, y, in, out);
        insert(index, p);
    }

    inline void erase (size_t index) {
        pList.erase(pList.begin() + index);
    }

    inline size_t size () {
        return pList.size();
    }

    inline ACurvePoint &point (size_t index) {
        if(index < pList.size()) {
            return pList[index];
        }
    }

    inline void point (float pos, size_t &index, AVector3D &value) {
        for(unsigned int i = 0; i < pList.size(); i++) {
            value       = pList[i].mY;
            index       = i;
            int o       = i - 1;
            if(o >= 0) {
                if(pList[o].mX <= pos && pos <= pList[i].mX) {
                    float u         = (pos - pList[o].mX) / (pList[i].mX - pList[o].mX);

                    AVector3D p1    = pList[o].mY;
                    AVector3D p2    = pList[i].mY;

                    AVector3D s1    = p1 + AVector3D(tan(-pList[o].mO.x * DEG2RAD),
                                                     tan(-pList[o].mO.y * DEG2RAD),
                                                     tan(-pList[o].mO.z * DEG2RAD));

                    AVector3D s2    = p2 - AVector3D(tan( pList[i].mI.x * DEG2RAD),
                                                     tan( pList[i].mI.y * DEG2RAD),
                                                     tan( pList[i].mI.z * DEG2RAD));

                    AVector3D a     = p2 * pow(u, 3);
                    AVector3D b     = s2 * (3 * pow(u, 2) * (1 - u));
                    AVector3D c     = s1 * (3 * u * pow((1 - u), 2));
                    AVector3D d     = p1 * (pow((1 - u), 3));

                    value           = (a + b) + (c + d);
                    return;
                }
            }
        }
    }

    inline void sort (unsigned int &index) {
        for(unsigned int i = 0; i < pList.size(); i++) {
            ACurvePoint p1  = pList[i];
            unsigned int o          = i + 1;
            if(o < pList.size()) {
                ACurvePoint p2  = pList[o];
                if(p1.mX > p2.mX) {
                    if(index == i)
                        index       = o;
                    else if(index == o)
                        index       = i;

                    pList[i]    = p2;
                    pList[o]    = p1;
                    sort(index);
                }
            }
        }
    }

    vector_point            pList;
};

#endif /* CURVE_H_HEADER_INCLUDED */
