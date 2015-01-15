#include "avariant.h"

#include <sstream>

bool convert(const AVariant::Data &data, AVariant::Type type, void *value) {
    if(!value)
        return false;

    bool result = true;

    switch(type) {
    case AVariant::BOOL: {
        bool *r     = static_cast<bool *>(value);
        switch(data.type) {
        case AVariant::BOOL:    { *r    = data.base.b; } break;
        case AVariant::INT:     { *r    = (data.base.i != 0); } break;
        case AVariant::FLOAT:   { *r    = (data.base.f != 0); } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::INT: {
        int *r      = static_cast<int *>(value);
        switch(data.type) {
        case AVariant::BOOL:    { *r    = (data.base.b) ? 1 : 0; } break;
        case AVariant::FLOAT:   { *r    = (int)data.base.f; } break;
        case AVariant::INT:     { *r    = data.base.i; } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::FLOAT: {
        float *r    = static_cast<float *>(value);
        switch(data.type) {
        case AVariant::BOOL:    { *r    = float(data.base.b); } break;
        case AVariant::FLOAT:   { *r    = data.base.f; } break;
        case AVariant::INT:     { *r    = float(data.base.i); } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::STRING: {
        std::string *r  = static_cast<std::string *>(value);
        std::ostringstream ss;
        switch(data.type) {
        case AVariant::BOOL:    { *r    = (data.base.b) ? "true" : "false"; } break;
        case AVariant::FLOAT:   { ss << data.base.f; *r = ss.str(); } break;
        case AVariant::INT:     { ss << data.base.i; *r = ss.str(); } break;
        case AVariant::STRING:  { *r    = data.s; } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::VECTOR: {
        AVector3D *r    = static_cast<AVector3D *>(value);
        switch(data.type) {
        case AVariant::FLOAT:   { *r    = AVector3D(data.base.f); } break;
        case AVariant::INT:     { *r    = AVector3D(data.base.i); } break;
        case AVariant::VECTOR:  { *r    = data.v; } break;
        case AVariant::COLOR:   { *r    = AVector3D(data.c.x, data.c.y, data.c.z); } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::COLOR: {
        AVariant::Color *r  = static_cast<AVariant::Color *>(value);
        switch(data.type) {
        case AVariant::FLOAT:   { *r    = AVariant::Color(data.base.f); } break;
        case AVariant::INT:     { *r    = AVariant::Color(data.base.i); } break;
        case AVariant::VECTOR:  { *r    = AVariant::Color(data.v, 1.0); } break;
        case AVariant::COLOR:   { *r    = data.c; } break;
        default:                { result    = false; } break;
        }
    } break;
    default: break;
    }

    return result;
}


template <typename T>
inline T aConversionHelper(AVariant::Data &data, AVariant::Type type, T value) {
    if(data.shared) {
        return *static_cast<T *>(data.base.so);
    }

    T result = value;
    convert(data, type, &result);

    return result;
}

AVariant::Data::Data() {
    type    = AVariant::ANY;
    base.b  = false;
    base.i  = 0;
    base.f  = 0.0f;
    base.so = 0;
    shared  = false;
}

AVariant::AVariant() {

}

AVariant::AVariant(Type type) {
    mData.type      = type;
}

AVariant::AVariant(bool value) {
    mData.type      = BOOL;
    mData.base.b    = value;
}

AVariant::AVariant(int value) {
    mData.type       = INT;
    mData.base.i     = value;
}

AVariant::AVariant(float value) {
    mData.type       = FLOAT;
    mData.base.f     = value;
}

AVariant::AVariant(const char *value) {
    mData.type       = STRING;
    mData.s          = value;
}

AVariant::AVariant(const std::string &value) {
    mData.type       = STRING;
    mData.s          = value;
}

AVariant::AVariant(const AVector3D &value) {
    mData.type       = VECTOR;
    mData.v          = value;
}

AVariant::AVariant(const Color &value) {
    mData.type       = COLOR;
    mData.c          = value;
}

AVariant::AVariant(bool *value) {
    mData.type       = BOOL;
    mData.base.so    = value;
    mData.shared     = true;
}

AVariant::AVariant(int *value) {
    mData.type       = INT;
    mData.base.so    = value;
    mData.shared     = true;
}

AVariant::AVariant(float *value) {
    mData.type       = FLOAT;
    mData.base.so    = value;
    mData.shared     = true;
}

AVariant::AVariant(std::string *value) {
    mData.type       = STRING;
    mData.base.so    = value;
    mData.shared     = true;
}

AVariant::AVariant(AVector3D *value) {
    mData.type       = VECTOR;
    mData.base.so    = value;
    mData.shared     = true;
}

AVariant::AVariant(Color *value) {
    mData.type       = COLOR;
    mData.base.so    = value;
    mData.shared     = true;
}

AVariant::AVariant(AObject *value) {
    mData.type       = OBJECT;
    mData.base.so    = value;
    mData.shared     = true;
}

AVariant::~AVariant() {
}

AVariant AVariant::operator=(const AVariant &value) {
    if(mData.shared) {
        convert(value.mData, mData.type, mData.base.so);
    } else {
        mData   = value.mData;
    }
    return *this;
}

AVariant::Type AVariant::type() {
    return mData.type;
}

bool AVariant::isConvertible() {
    return false;
}

// Conversion and getters
const bool AVariant::toBool() {
    return aConversionHelper<bool>(mData, BOOL, true);
}

const int AVariant::toInt() {
    return aConversionHelper<int>(mData, INT, 0);
}

const float AVariant::toFloat() {
    return aConversionHelper<float>(mData, FLOAT, 0.0f);
}

const std::string AVariant::toString() {
    return aConversionHelper<std::string>(mData, STRING, "");
}

const AVector3D AVariant::toVector() {
    return aConversionHelper<AVector3D>(mData, VECTOR, AVector3D());
}

const AVariant::Color AVariant::toColor() {
    return aConversionHelper<Color>(mData, COLOR, Color());
}
