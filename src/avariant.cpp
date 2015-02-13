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
    case AVariant::VECTOR: {
        AVector3D *r    = static_cast<AVector3D *>(value);
        switch(data.type) {
        case AVariant::FLOAT:   { *r    = AVector3D(SHARED(float) : data.base.f); } break;
        case AVariant::INT:     { *r    = AVector3D(SHARED(int) : data.base.i); } break;
        case AVariant::VECTOR:  { *r    = SHARED(AVector3D) : data.v; } break;
        case AVariant::COLOR:   { AVector4D v   = SHARED(AVector4D) : data.c; *r = AVector3D(v.x, v.y, v.z); } break;
        default:                { result    = false; } break;
        }
    } break;
    case AVariant::COLOR: {
        AVariant::Color *r  = static_cast<AVariant::Color *>(value);
        switch(data.type) {
        case AVariant::FLOAT:   { *r    = AVariant::Color(SHARED(float) : data.base.f); } break;
        case AVariant::INT:     { *r    = AVariant::Color(SHARED(int) : data.base.i); } break;
        case AVariant::VECTOR:  { *r    = AVariant::Color(SHARED(AVector3D) : data.v, 1.0); } break;
        case AVariant::COLOR:   { *r    = SHARED(AVector4D) : data.c; } break;
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
    type    = AVariant::ANY;
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

    if(value.size() == 3) {
        AVector3D v;
        if(toFloatArray(v.v, value)) {
            mData.type  = VECTOR;
            mData.v     = v;
            mData.l.clear();
        }
    } else if(value.size() == 4) {
        Color c;
        if(toFloatArray(c.v, value)) {
            mData.type  = COLOR;
            mData.c     = c;
            mData.l.clear();
        }
    }
}

AVariant::AVariant(const AVector3D &value) {
    mData.type      = VECTOR;
    mData.v         = value;
}

AVariant::AVariant(const Color &value) {
    mData.type      = COLOR;
    mData.c         = value;
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

AVariant::AVariant(AVector3D *value) {
    mData.type      = VECTOR;
    mData.base.so   = value;
    mData.shared    = true;
}

AVariant::AVariant(Color *value) {
    mData.type      = COLOR;
    mData.base.so   = value;
    mData.shared    = true;
}

AVariant::~AVariant() {
}

AVariant &AVariant::operator=(const AVariant &value) {
    /// \todo: Need to think about case when value is shared
    if(mData.shared) {
        convert(value.mData, mData.type, mData.base.so);
    } else {
        mData   = value.mData;
    }
    return *this;
}

bool AVariant::operator==(const AVariant &right) const {
    switch(mData.type) {
        case ANY:   return (mData.type  == right.mData.type);
        case BOOL:  return toBool()     == right.toBool();
        case INT:   return toInt()      == right.toInt();
        case FLOAT: return toFloat()    == right.toFloat();
        case STRING:return toString()   == right.toString();
        case VECTOR:{
            AVector3D v1    = toVector();
            AVector3D v2    = right.toVector();
            return v1 == v2;
        }
        case COLOR: {
            Color c1    = toColor();
            Color c2    = right.toColor();
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

const AVector3D AVariant::toVector() const {
    return aConversionHelper<AVector3D>(mData, VECTOR, AVector3D());
}

const AVariant::Color AVariant::toColor() const {
    return aConversionHelper<Color>(mData, COLOR, Color());
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
        case VECTOR: {
            AVariantList l;
            l.push_back(mData.v.x);
            l.push_back(mData.v.y);
            l.push_back(mData.v.z);
            return l;
        }
        case COLOR: {
            AVariantList l;
            l.push_back(mData.c.x);
            l.push_back(mData.c.y);
            l.push_back(mData.c.z);
            l.push_back(mData.c.w);
            return l;
        }
        default: break;
    }

    return AVariantList();
}

void AVariant::appendProperty(const AVariant &value, const string &name) {
    AVariant v   = value;
    if(value.mData.type == AVariant::LIST) {
        if(value.mData.l.size() == 3) {
            AVector3D vector;
            if(toFloatArray(vector.v, value.mData.l)) {
                v = vector;
            }
        } else if(value.mData.l.size() == 4) {
            Color color;
            if(toFloatArray(color.v, value.mData.l)) {
                v = color;
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
