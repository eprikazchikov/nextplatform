#ifndef ARANGE_H_HEADER_INCLUDED
#define ARANGE_H_HEADER_INCLUDED

struct ARange {
    inline ARange       () : mMin(0), mMax(1)                         { }
    inline ARange       (float min, float max) : mMin(min), mMax(max) { }

    void                seed(unsigned int s) {
        srand(s);
    }

    inline float        frand() {
        return ((mMax - mMin)*((float)rand()/RAND_MAX)) + mMin;
    }

    union {
        struct {
            float   mMin;
            float   mMax;
        };
        float v[2];
    };

};

#endif /* ARANGE_H_HEADER_INCLUDED */
