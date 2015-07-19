#include "avariant.h"

#define SHARED(type) (data.shared) ? *static_cast<type *>(data.base.so)

#define STRUCTURE "__S__"
#define DATA "__D__"

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
        case AVariant::FLOAT:   { string str    = to_string(SHARED(float) : data.base.f); str.erase(str.find_last_not_of('0') + 1, string::npos ); *r    = str; } break;
        case AVariant::INT:     { *r    = to_string(SHARED(int) : data.base.i);     } break;
        case AVariant::STRING:  { *r    = SHARED(string) : data.s; } break;
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

template <typename T>
inline T aFloatArrayHelper(const AVariant::Data &data) {
    T result;
    if(data.shared) {
        result  = *static_cast<T *>(data.base.so);
    } else {
        int i = 0;
        for(auto &it : data.l) {
            result[i]   = it.toFloat();
            i++;
        }
    }
    return result;
}

template <typename T>
inline void aValueToSharedHelper(const AVariant::Data &data, const AVariant::Data &value) {
    *static_cast<T *>(data.base.so) = aFloatArrayHelper<T>(value);
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
}


AVariant::AVariant(const AVector2D &value) {
    mData.type      = VECTOR2D;
    for(int i = 0; i < 2; i++) {
        mData.l.push_back(value[i]);
    }
}

AVariant::AVariant(const AVector3D &value) {
    mData.type      = VECTOR3D;
    for(int i = 0; i < 3; i++) {
        mData.l.push_back(value[i]);
    }
}

AVariant::AVariant(const AVector4D &value) {
    mData.type      = VECTOR4D;
    for(int i = 0; i < 4; i++) {
        mData.l.push_back(value[i]);
    }
}

AVariant::AVariant(const AQuaternion &value) {
    mData.type      = QUATERNION;
    for(int i = 0; i < 4; i++) {
        mData.l.push_back(value[i]);
    }
}

AVariant::AVariant(const AMatrix3D &value) {
    mData.type      = MATRIX3D;
    for(int i = 0; i < 9; i++) {
        mData.l.push_back(value[i]);
    }
}

AVariant::AVariant(const AMatrix4D &value) {
    mData.type      = MATRIX4D;
    for(int i = 0; i < 16; i++) {
        mData.l.push_back(value[i]);
    }
}

AVariant::AVariant(const ACurve &value) {
    mData.type      = CURVE;
    for(const ACurvePoint &it : value.mList) {
        mData.l.push_back(it.mX);
        mData.l.push_back(it.mY);
        mData.l.push_back(it.mI);
        mData.l.push_back(it.mO);
    }
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

AVariant::AVariant(AQuaternion *value) {
    mData.type      = QUATERNION;
    mData.base.so   = value;
    mData.shared    = true;
}

AVariant::AVariant(AMatrix3D *value) {
    mData.type      = MATRIX3D;
    mData.base.so   = value;
    mData.shared    = true;
}

AVariant::AVariant(AMatrix4D *value) {
    mData.type      = MATRIX4D;
    mData.base.so   = value;
    mData.shared    = true;
}

AVariant::AVariant(ACurve *value) {
    mData.type      = CURVE;
    mData.base.so   = value;
    mData.shared    = true;
}

AVariant::~AVariant() {
}

AVariant AVariant::operator*() {
    AVariant result;
    result.mData.type  = mData.type;
    switch(mData.type) {
        case BOOL:      result.mData.base.b = toBool();     break;
        case INT:       result.mData.base.i = toInt();      break;
        case FLOAT:     result.mData.base.f = toFloat();    break;
        case STRING:    result.mData.s      = toString();   break;
        case VECTOR2D:
        case VECTOR3D:
        case VECTOR4D:
        case QUATERNION:
        case MATRIX3D:
        case MATRIX4D:
        case CURVE:
        case LIST:      result.mData.l      = toList();     break;
        case MAP:       result.mData.m      = toMap();      break;
        default: {
            break;
        }
    }
    return result;
}

AVariant &AVariant::operator=(const AVariant &value) {
    if(mData.shared) {
        switch(mData.type) {
            case VECTOR2D: {
                aValueToSharedHelper<AVector2D>     (mData, value.mData);
            } break;
            case VECTOR3D: {
                aValueToSharedHelper<AVector3D>     (mData, value.mData);
            } break;
            case VECTOR4D: {
                aValueToSharedHelper<AVector4D>     (mData, value.mData);
            } break;
            case QUATERNION: {
                aValueToSharedHelper<AQuaternion>   (mData, value.mData);
            } break;
            case MATRIX3D: {
                aValueToSharedHelper<AMatrix3D>     (mData, value.mData);
            } break;
            case MATRIX4D: {
                aValueToSharedHelper<AMatrix4D>     (mData, value.mData);
            } break;
            case CURVE: {
                //aValueToSharedHelper<ACurve>        (mData, value.mData);
            } break;
            default: {
                convert(value.mData, mData.type, mData.base.so);
            } break;
        }
    } else {
        mData   = value.mData;
    }
    return *this;
}

bool AVariant::operator==(const AVariant &right) const {
    switch(mData.type) {
        case NONE:          return (mData.type  == right.mData.type);
        case BOOL:          return toBool()     == right.toBool();
        case INT:           return toInt()      == right.toInt();
        case FLOAT:         return toFloat()    == right.toFloat();
        case STRING:        return toString()   == right.toString();
        case MAP:           return toMap()      == right.toMap();
        case VECTOR2D:      return toVector2D() == right.toVector2D();
        case VECTOR3D:      return toVector3D() == right.toVector3D();
        case VECTOR4D:      return toVector4D() == right.toVector4D();
        case QUATERNION:    return toQuaternion()   == right.toQuaternion();
        case MATRIX3D:      return toMatrix3D() == right.toMatrix3D();
        case MATRIX4D:      return toMatrix4D() == right.toMatrix4D();
        case CURVE:         return toCurve()    == right.toCurve();
        default: {
            return toList()     == right.toList();
        }
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

const AVariant::AVariantMap AVariant::toMap() const {
    AVariantMap result = mData.m;

    switch(mData.type) {
        case VECTOR2D:
        case VECTOR3D:
        case VECTOR4D:
        case QUATERNION:
        case MATRIX3D:
        case MATRIX4D:
        case CURVE: {
            result[STRUCTURE]   = static_cast<int>(mData.type);
            result[DATA]        = toList();
        } break;
        default: break;
    }

    return result;
}

const AVariant::AVariantList AVariant::toList() const {
    if(!mData.shared) {
        switch(mData.type) {
            case VECTOR2D:
            case VECTOR3D:
            case VECTOR4D:
            case QUATERNION:
            case MATRIX3D:
            case MATRIX4D:
            case CURVE:
            case LIST: {
                return mData.l;
            }
            default: break;
        }
    } else {
        switch(mData.type) {
            case VECTOR2D:  return AVariant(*static_cast<AVector2D *>(mData.base.so)).toList();
            case VECTOR3D:  return AVariant(*static_cast<AVector3D *>(mData.base.so)).toList();
            case VECTOR4D:  return AVariant(*static_cast<AVector4D *>(mData.base.so)).toList();
            case QUATERNION:return AVariant(*static_cast<AQuaternion *>(mData.base.so)).toList();
            case MATRIX3D:  return AVariant(*static_cast<AMatrix3D *>(mData.base.so)).toList();
            case MATRIX4D:  return AVariant(*static_cast<AMatrix4D *>(mData.base.so)).toList();
            case CURVE:     return AVariant(*static_cast<ACurve *>(mData.base.so)).toList();
            default: break;
        }
    }

    return AVariantList();
}

const AVector2D AVariant::toVector2D() const {
    return aFloatArrayHelper<AVector2D>(mData);
}

const AVector3D AVariant::toVector3D() const {
    return aFloatArrayHelper<AVector3D>(mData);
}

const AVector4D AVariant::toVector4D() const {
    return aFloatArrayHelper<AVector4D>(mData);
}

const AQuaternion AVariant::toQuaternion() const {
    return aFloatArrayHelper<AQuaternion>(mData);
}

const AMatrix3D AVariant::toMatrix3D() const {
    return aFloatArrayHelper<AMatrix3D>(mData);
}

const AMatrix4D AVariant::toMatrix4D() const {
    return aFloatArrayHelper<AMatrix4D>(mData);
}

const ACurve AVariant::toCurve() const {
    ACurve result;
    for(auto it = mData.l.begin(); it != mData.l.end(); it++) {
        float x     = (*it).toFloat();
        it++;
        AVector3D y = (*it).toVector3D();
        it++;
        AVector3D i = (*it).toVector3D();
        it++;
        AVector3D o = (*it).toVector3D();

        result.append(x, y, i, o);
    }
    return result;
}

void AVariant::appendProperty(const AVariant &value, const string &name) {
    if(mData.type == AVariant::MAP || mData.type >= VECTOR2D) {
        if(name == STRUCTURE) {
            mData.type  = static_cast<Type>(value.toInt());
        } else if(name == DATA) {
            mData.l     = value.toList();
        } else {
            mData.shared    = value.mData.shared;
            mData.m[name]   = value;
        }
    } else if(mData.type == AVariant::LIST) {
        mData.l.push_back(value);
    }
}
