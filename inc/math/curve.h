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

    inline ACurvePoint (float x, const AVector3D &y, const AVector3D &i, const AVector3D &o) {
        mX  = x; mY = y; mI = i; mO = o;
    }

    bool operator== (const ACurvePoint &v) const {
        return (mX == v.mX) && (mY == v.mY) && (mI == v.mI) && (mO == v.mO);
    }

    bool operator!= (const ACurvePoint &v) const {
        return !(*this == v);
    }

    float           mX;
    AVector3D       mY;
    AVector3D       mI;
    AVector3D       mO;
};

struct ACurve {
    typedef std::vector<ACurvePoint>    PointsList;

    inline ACurve () {
    }

    inline ACurve (ACurvePoint &p) {
        append (p);
    }

    inline ACurve (float x, AVector3D &y, AVector3D &in, AVector3D &out) {
        append (x, y, in, out);
    }

    /// Comparison operators
    bool operator== (const ACurve &v) const {
        for(unsigned int i = 0; i < mList.size(); i++) {
            if(mList[i] != v.mList[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator!= (const ACurve &v) const {
        return !(*this == v);
    }

    inline void append (ACurvePoint &p) {
        mList.push_back(p);
    }

    inline void append (float x, const AVector3D &y, const AVector3D &in, const AVector3D &out) {
        ACurvePoint p(x, y, in, out);
        append(p);
    }

    inline void insert (size_t index, ACurvePoint &p) {
        mList.insert(mList.begin() + index, p);
    }

    inline void insert (size_t index, float x, const AVector3D &y, const AVector3D &in, const AVector3D &out) {
        ACurvePoint p(x, y, in, out);
        insert(index, p);
    }

    inline void erase (size_t index) {
        mList.erase(mList.begin() + index);
    }

    inline size_t size () {
        return mList.size();
    }

    inline ACurvePoint point (size_t index) {
        if(index < mList.size()) {
            return mList[index];
        }
        return ACurvePoint();
    }

    inline void point (float pos, size_t &index, AVector3D &value) {
        for(unsigned int i = 0; i < mList.size(); i++) {
            value       = mList[i].mY;
            index       = i;
            int o       = i - 1;
            if(o >= 0) {
                if(mList[o].mX <= pos && pos <= mList[i].mX) {
                    float u         = (pos - mList[o].mX) / (mList[i].mX - mList[o].mX);

                    AVector3D p1    = mList[o].mY;
                    AVector3D p2    = mList[i].mY;

                    AVector3D s1    = p1 + AVector3D(tan(-mList[o].mO.x * DEG2RAD),
                                                     tan(-mList[o].mO.y * DEG2RAD),
                                                     tan(-mList[o].mO.z * DEG2RAD));

                    AVector3D s2    = p2 - AVector3D(tan( mList[i].mI.x * DEG2RAD),
                                                     tan( mList[i].mI.y * DEG2RAD),
                                                     tan( mList[i].mI.z * DEG2RAD));

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
        for(unsigned int i = 0; i < mList.size(); i++) {
            ACurvePoint p1  = mList[i];
            unsigned int o          = i + 1;
            if(o < mList.size()) {
                ACurvePoint p2  = mList[o];
                if(p1.mX > p2.mX) {
                    if(index == i)
                        index       = o;
                    else if(index == o)
                        index       = i;

                    mList[i]    = p2;
                    mList[o]    = p1;
                    sort(index);
                }
            }
        }
    }

    PointsList  mList;
};

#endif /* CURVE_H_HEADER_INCLUDED */
