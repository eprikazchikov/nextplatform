#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "core/variant.h"

class NEXT_LIBRARY_EXPORT KeyFrame {
public:
    enum Type {
        Linear                  = 0,
        Cubic
    };

public:
    KeyFrame                    ();

    KeyFrame                    (float position, Variant &value);

    KeyFrame                    (float position, Variant &value, Variant &support);

    float                       mPosition;

    Type                        mType;

    Variant                     mValue;

    Variant                     mSupport;
};

#endif // KEYFRAME_H
