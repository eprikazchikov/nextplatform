#include "avariant.h"

#include <sstream>

bool convert(const AVariant::Data &data, AVariant::Type type, void *value) {
    if(!value)
        return false;

    bool result = true;

    switch(data.type) {
    case AVariant::BOOL: {
        switch(type) {
        case AVariant::BOOL:    { *static_cast<bool *>(value) = data.base.b; } break;
        case AVariant::INT:     { *static_cast<bool *>(value) = (data.base.i != 0); } break;
        case AVariant::FLOAT:   { *static_cast<bool *>(value) = (data.base.f != 0); } break;
        case AVariant::OBJECT:  { *static_cast<bool *>(value) = (data.base.so != 0); } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::INT: {
        switch(type) {
        case AVariant::BOOL:    { *static_cast<int *>(value) = (data.base.b) ? 1 : 0; } break;
        case AVariant::FLOAT:   { *static_cast<int *>(value) = (int)data.base.f; } break;
        case AVariant::INT:     { *static_cast<int *>(value) = data.base.i; } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::FLOAT: {
        switch(type) {
        case AVariant::BOOL:    { *static_cast<float *>(value)  = data.base.b; } break;
        case AVariant::FLOAT:   { *static_cast<float *>(value)  = data.base.f; } break;
        case AVariant::INT:     { *static_cast<float *>(value)  = data.base.i; } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::STRING: {
        std::ostringstream ss;
        switch(type) {
        case AVariant::BOOL:    { *static_cast<std::string *>(value)    = (data.base.b) ? "true" : "false"; } break;
        case AVariant::FLOAT:   { ss << data.base.f; *static_cast<std::string *>(value) = ss.str(); } break;
        case AVariant::INT:     { ss << data.base.i; *static_cast<std::string *>(value) = ss.str(); } break;
        case AVariant::STRING:  { *static_cast<std::string *>(value)    = data.s; } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::VECTOR: {
        switch(type) {
        case AVariant::FLOAT:   { *static_cast<AVector3D *>(value)      = AVector3D(data.base.f); } break;
        case AVariant::INT:     { *static_cast<AVector3D *>(value)      = AVector3D(data.base.i); } break;
        case AVariant::VECTOR:  { *static_cast<AVector3D *>(value)      = data.v; } break;
        case AVariant::COLOR:   { *static_cast<AVector3D *>(value)      = AVector3D(data.c.x, data.c.y, data.c.z); } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::COLOR: {
        switch(type) {
        case AVariant::FLOAT:   { *static_cast<AVariant::Color *>(value)    = AVariant::Color(data.base.f); } break;
        case AVariant::INT:     { *static_cast<AVariant::Color *>(value)    = AVariant::Color(data.base.i); } break;
        case AVariant::VECTOR:  { *static_cast<AVariant::Color *>(value)    = AVariant::Color(data.v, 1.0); } break;
        case AVariant::COLOR:   { *static_cast<AVariant::Color *>(value)    = data.c; } break;
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
bool AVariant::toBool() {
    return aConversionHelper<bool>(mData, BOOL, true);
}

int AVariant::toInt() {
    return aConversionHelper<int>(mData, INT, 0);
}

float AVariant::toFloat() {
    return aConversionHelper<float>(mData, FLOAT, 0.0f);
}

std::string AVariant::toString() {
    return aConversionHelper<std::string>(mData, STRING, "");
}

AVector3D AVariant::toVector() {
    return aConversionHelper<AVector3D>(mData, VECTOR, AVector3D());
}

AVariant::Color AVariant::toColor() {
    return aConversionHelper<Color>(mData, COLOR, Color());
}
