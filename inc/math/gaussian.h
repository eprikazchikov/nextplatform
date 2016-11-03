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

    (c) Copyright: 2008-2014 Evgeny Prikazchikov
*/

#ifndef GAUSSIAN_H_HEADER_INCLUDED
#define GAUSSIAN_H_HEADER_INCLUDED

#include <cstdint>
#include <cmath>

#include "vector2.h"

struct AGausian {
    static int computeKernel(float radius, float *samples, uint8_t maxSamples) {
        if(samples) {
            int32_t integerRadius   = maxSamples - 1;

            uint32_t count  = 0;
            float sum       = 0.0f;
            for(int32_t i = -integerRadius; i <= integerRadius; i += 2) {
                float weight= normalDistribution(i, 0, radius);

                samples[count]  = weight;
                sum            += weight;
                count++;
            }

            float invSum = 1.0f / sum;
            for(uint32_t i = 0; i < count; ++i) {
                samples[i]     *= invSum;
            }

            return count;
        }
        return 0;
    }

    static float normalDistribution(float x, float mean, float variance) {
        return std::exp(-((x - mean) * (x - mean)) / (2.0f * variance));
    }

};

#endif /* GAUSSIAN_H_HEADER_INCLUDED */
