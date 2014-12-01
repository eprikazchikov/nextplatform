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

#ifndef PERLIN_H_HEADER_INCLUDED
#define PERLIN_H_HEADER_INCLUDED

struct APerlin {
    static inline float compile(float x, float y) {
        float intx          = int(x);
        float fractionalx   = x - intx;

        float inty          = int(y);
        float fractionaly   = y - inty;

        float v1    = smoothed(intx,     inty);
        float v2    = smoothed(intx + 1, inty);
        float v3    = smoothed(intx,     inty + 1);
        float v4    = smoothed(intx + 1, inty + 1);

        float i1    = MIX(v1, v2, fractionalx);
        float i2    = MIX(v3, v4, fractionalx);

        return corp(i1, i2, fractionaly);
    }

    static inline float smoothed(float x, float y) {
        float corners = (noise2d(x - 1, y - 1) + noise2d(x + 1, y - 1) + noise2d(x - 1, y + 1) + noise2d(x + 1, y + 1) ) / 16;
        float sides   = (noise2d(x - 1, y) + noise2d(x + 1, y) + noise2d(x, y - 1) + noise2d(x, y + 1) ) /  8;
        float center  =  noise2d(x, y) / 4;

        return corners + sides + center;
    }

    static inline float noise2d(float x, float y) {
        int n   = x + y * 57;
        n       = (n << 13) ^ n;
        return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
    }

    static float corp(float a, float b, float x) {
        float f = (1.0f - (float)cos(x * PI)) * 0.5f;
        return MIX(a, b, f);
    }
};

#endif /* PERLIN_H_HEADER_INCLUDED */
