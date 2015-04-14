#include "avariant.h"

#define SHARED(type) (data.shared) ? *static_cast<type *>(data.base.so)

bool convert(const AVariant::Data &data, AVariant::Type type, void *value) {
    if(!value)
        return false;

    bool result = true;

    switch(type) {
    case AVariant::BOOL: {
        bool *r     = static_cast<bool *>(value);
        switch(data.type) {
        case AVariant::BOOL:    { *r    = SHARED(bool) : data.base.b; } break;
        case AVariant::INT:     { *r    = (SHARED(int) : data.base.i) != 0; } break;
        case AVariant::FLOAT:   { *r    = (SHARED(float) : data.base.f) != 0; } break;
        case AVariant::STRING:  { string s  = (SHARED(string) : data.s); *r = (s != "false" || s != "0" || !s.empty()); }  break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::INT: {
        int *r      = static_cast<int *>(value);
        switch(data.type) {
        case AVariant::BOOL:    { *r    = (SHARED(bool) : data.base.b) ? 1 : 0; } break;
        case AVariant::FLOAT:   { float f   = SHARED(float) : data.base.f; *r = int(f); f -= *r; *r += (f >= 0.5f) ? 1 : 0; } break;
        case AVariant::INT:     { *r    = SHARED(int) : data.base.i; } break;
        case AVariant::STRING:  { *r    = stoi(SHARED(string) : data.s); } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::FLOAT: {
        float *r    = static_cast<float *>(value);
        switch(data.type) {
        case AVariant::BOOL:    { *r    = float(SHARED(bool) : data.base.b); } break;
        case AVariant::FLOAT:   { *r    = SHARED(float) : data.base.f; } break;
        case AVariant::INT:     { *r    = float(SHARED(int) : data.base.i); } break;
        case AVariant::STRING:  { *r    = stof(SHARED(string) : data.s); } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::STRING: {
        string *r  = static_cast<string *>(value);
        switch(data.type) {
        case AVariant::BOOL:    { *r    = (SHARED(bool) : data.base.b) ? "true" : "false"; } break;
        case AVariant::FLOAT:   { *r    = to_string(SHARED(float) : data.base.f);   } break;
        case AVariant::INT:     { *r    = to_string(SHARED(int) : data.base.i);     } break;
        case AVariant::STRING:  { *r    = SHARED(string) : data.s; } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::VECTOR2D: {
        AVector2D *r    = static_cast<AVector2D *>(value);
        switch(data.type) {
        case AVariant::FLOAT:   { *r    = AVector2D(SHARED(float) : data.base.f); } break;
        case AVariant::INT:     { *r    = AVector2D(SHARED(int) : data.base.i); } break;
        case AVariant::VECTOR2D:{ *r    = SHARED(AVector2D) : AVector2D(data.v.x, data.v.y); } break;
        case AVariant::VECTOR3D:{ AVector3D v   = SHARED(AVector3D) : AVector3D(data.v.x, data.v.y, data.v.z); *r = AVector2D(v.x, v.y); } break;
        case AVariant::VECTOR4D:{ AVector4D v   = SHARED(AVector4D) : data.v; *r = AVector2D(v.x, v.y); } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::VECTOR3D: {
        AVector3D *r    = static_cast<AVector3D *>(value);
        switch(data.type) {
        case AVariant::FLOAT:   { *r    = AVector3D(SHARED(float) : data.base.f); } break;
        case AVariant::INT:     { *r    = AVector3D(SHARED(int) : data.base.i); } break;
        case AVariant::VECTOR2D:{ AVector2D v   = SHARED(AVector2D) : AVector2D(data.v.x, data.v.y); *r = AVector3D(v.x, v.y, 0.0); } break;
        case AVariant::VECTOR3D:{ *r    = SHARED(AVector3D) : AVector3D(data.v.x, data.v.y, data.v.z); } break;
        case AVariant::VECTOR4D:{ AVector4D v   = SHARED(AVector4D) : data.v; *r = AVector3D(v.x, v.y, v.z); } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::VECTOR4D: {
        AVector4D *r    = static_cast<AVector4D *>(value);
        switch(data.type) {
        case AVariant::FLOAT:   { *r    = AVector4D(SHARED(float) : data.base.f); } break;
        case AVariant::INT:     { *r    = AVector4D(SHARED(int) : data.base.i); } break;
        case AVariant::VECTOR2D:{ AVector2D v   = SHARED(AVector2D) : AVector2D(data.v.x, data.v.y); *r = AVector4D(v.x, v.y, 0.0, 1.0); } break;
        case AVariant::VECTOR3D:{ AVector3D v   = SHARED(AVector3D) : AVector3D(data.v.x, data.v.y, data.v.z); *r = AVector4D(v.x, v.y, v.z, 1.0); } break;
        case AVariant::VECTOR4D:{ *r    = SHARED(AVector4D) : data.v; } break;
        default:                { result    = false; } break;
        }
    } break;
    default: break;
    }

    return result;
}

template <typename T>
inline T aConversionHelper(const AVariant::Data &data, AVariant::Type type, T value) {
    T result = value;
    convert(data, type, &result);

    return result;
}

AVariant::Data::Data() {
    type    = AVariant::NONE;
    base.b  = false;
    base.i  = 0;
    base.f  = 0.0f;
    base.so = 0;
    shared  = false;
}

AVariant::AVariant() {

}

AVariant::AVariant(const AVariant &copy) {
    mData.type  = copy.mData.type;
    *this       = copy;
}

AVariant::AVariant(Type type) {
    mData.type      = type;
}

AVariant::AVariant(bool value) {
    mData.type      = BOOL;
    mData.base.b    = value;
}

AVariant::AVariant(int value) {
    mData.type      = INT;
    mData.base.i    = value;
}

AVariant::AVariant(float value) {
    mData.type      = FLOAT;
    mData.base.f    = value;
}

AVariant::AVariant(const char *value) {
    mData.type      = STRING;
    mData.s         = value;
}

AVariant::AVariant(const string &value) {
    mData.type      = STRING;
    mData.s         = value;
}

AVariant::AVariant(const AVariantMap &value) {
    mData.type      = MAP;
    mData.m         = value;
}

AVariant::AVariant(const AVariantList &value) {
    mData.type      = LIST;
    mData.l         = value;

    AVector4D v;
    if(toFloatArray(v.v, value)) {
        switch (value.size()) {
            case 2: mData.type  = VECTOR2D; break;
            case 3: mData.type  = VECTOR3D; break;
            case 4: mData.type  = VECTOR4D; break;
            default: return;
        }
        mData.v = v;
        mData.l.clear();
    }
}

AVariant::AVariant(const AVector2D &value) {
    mData.type      = VECTOR2D;
    mData.v         = AVector4D(value, 0.0f, 1.0f);
}


AVariant::AVariant(const AVector3D &value) {
    mData.type      = VECTOR3D;
    mData.v         = AVector4D(value, 1.0f);
}

AVariant::AVariant(const AVector4D &value) {
    mData.type      = VECTOR4D;
    mData.v         = value;
}

AVariant::AVariant(bool *value) {
    mData.type      = BOOL;
    mData.base.so   = value;
    mData.shared    = true;
}

AVariant::AVariant(int *value) {
    mData.type      = INT;
    mData.base.so   = value;
    mData.shared    = true;
}

AVariant::AVariant(float *value) {
    mData.type      = FLOAT;
    mData.base.so   = value;
    mData.shared    = true;
}

AVariant::AVariant(string *value) {
    mData.type      = STRING;
    mData.base.so   = value;
    mData.shared    = true;
}

AVariant::AVariant(AVector2D *value) {
    mData.type      = VECTOR2D;
    mData.base.so   = value;
    mData.shared    = true;
}

AVariant::AVariant(AVector3D *value) {
    mData.type      = VECTOR3D;
    mData.base.so   = value;
    mData.shared    = true;
}

AVariant::AVariant(AVector4D *value) {
    mData.type      = VECTOR4D;
    mData.base.so   = value;
    mData.shared    = true;
}

AVariant::~AVariant() {
}

AVariant &AVariant::operator=(const AVariant &value) {
    if(mData.shared) {
        convert(value.mData, mData.type, mData.base.so);
    } else {
        /// \todo: Need to think about case when value is shared
        mData   = value.mData;
    }
    return *this;
}

bool AVariant::operator==(const AVariant &right) const {
    switch(mData.type) {
        case NONE:  return (mData.type  == right.mData.type);
        case BOOL:  return toBool()     == right.toBool();
        case INT:   return toInt()      == right.toInt();
        case FLOAT: return toFloat()    == right.toFloat();
        case STRING:return toString()   == right.toString();
        case VECTOR2D: {
            AVector2D c1    = toVector2D();
            AVector2D c2    = right.toVector2D();
            return c1 == c2;
        }
        case VECTOR3D:{
            AVector3D v1    = toVector3D();
            AVector3D v2    = right.toVector3D();
            return v1 == v2;
        }
        case VECTOR4D: {
            AVector4D c1    = toVector4D();
            AVector4D c2    = right.toVector4D();
            return c1 == c2;
        }
        case MAP:   return toMap()      == right.toMap();
        case LIST:  return toList()     == right.toList();
        default: break;
    }
    return false;
}

bool AVariant::operator!=(const AVariant &right) const {
    return !(*this == right);
}

AVariant::Type AVariant::type() const {
    return mData.type;
}

bool AVariant::isShared() const {
    return mData.shared;
}

// Conversion and getters
const bool AVariant::toBool() const {
    return aConversionHelper<bool>(mData, BOOL, false);
}

const int AVariant::toInt() const {
    return aConversionHelper<int>(mData, INT, 0);
}

const float AVariant::toFloat() const {
    return aConversionHelper<float>(mData, FLOAT, 0.0f);
}

const string AVariant::toString() const {
    return aConversionHelper<string>(mData, STRING, "");
}

const AVector2D AVariant::toVector2D() const {
    return aConversionHelper<AVector2D>(mData, VECTOR2D, AVector2D());
}

const AVector3D AVariant::toVector3D() const {
    return aConversionHelper<AVector3D>(mData, VECTOR3D, AVector3D());
}

const AVector4D AVariant::toVector4D() const {
    return aConversionHelper<AVector4D>(mData, VECTOR4D, AVector4D());
}

const AVariant::AVariantMap AVariant::toMap() const {
    if(mData.type == MAP) {
        return mData.m;
    }
    return AVariantMap();
}

const AVariant::AVariantList AVariant::toList() const {
    switch(mData.type) {
        case LIST: return mData.l;
        case VECTOR2D: {
            AVariantList l;
            AVector2D v = toVector2D();
            l.push_back(v.x);
            l.push_back(v.y);
            return l;
        }
        case VECTOR3D: {
            AVariantList l;
            AVector3D v = toVector3D();
            l.push_back(v.x);
            l.push_back(v.y);
            l.push_back(v.z);
            return l;
        }
        case VECTOR4D: {
            AVariantList l;
            AVector4D v = toVector4D();
            l.push_back(v.x);
            l.push_back(v.y);
            l.push_back(v.z);
            l.push_back(v.w);
            return l;
        }
        default: break;
    }

    return AVariantList();
}

void AVariant::appendProperty(const AVariant &value, const string &name) {
    AVariant v   = value;
    if(value.mData.type == AVariant::LIST) {
        AVector4D vector;
        if(toFloatArray(vector.v, value.mData.l)) {
            switch(value.mData.l.size()) {
                case 2: {
                    v   = AVector2D(vector.x, vector.y);
                } break;
                case 3: {
                    v   = AVector3D(vector.x, vector.y, vector.z);
                } break;
                case 4: {
                    v   = AVector4D(vector.x, vector.y, vector.z, vector.w);
                } break;
                default: break;
            }
        }
    }

    if(mData.type == AVariant::MAP) {
        mData.m[name]   = v;
    } else if(mData.type == AVariant::LIST) {
        mData.l.push_back(v);
    }
}

inline bool AVariant::toFloatArray(float *v, const AVariantList &list) {
    char i  = 0;
    for(const auto &it: list) {
        if(it.mData.type == FLOAT) {
            v[i++]  = it.toFloat();
        } else {
            return false;
        }
    }
    return true;
}
