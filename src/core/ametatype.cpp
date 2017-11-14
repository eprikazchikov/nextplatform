#include "core/ametatype.h"

#include <list>

#include "math/amath.h"
#include "core/avariant.h"

#define DECLARE_BUILT_TYPE(TYPE) \
    { \
        TypeFuncs<TYPE>::size, \
        TypeFuncs<TYPE>::static_new, \
        TypeFuncs<TYPE>::construct, \
        TypeFuncs<TYPE>::static_delete, \
        TypeFuncs<TYPE>::destruct, \
        TypeFuncs<TYPE>::clone, \
        TypeFuncs<TYPE>::move, \
        TypeFuncs<TYPE>::compare, \
        TypeFuncs<TYPE>::index, \
        #TYPE \
    }

typedef map<uint32_t, AMetaType::Table> TypeMap;
typedef map<string, uint32_t>           NameMap;
typedef map<uint32_t, map<uint32_t, AMetaType::converterCallback>> ConverterMap;

uint32_t AMetaType::s_NextId = AMetaType::UserType;
static TypeMap s_Types = {
    {AMetaType::Bool,       DECLARE_BUILT_TYPE(bool)},
    {AMetaType::Int,        DECLARE_BUILT_TYPE(int)},
    {AMetaType::Double,     DECLARE_BUILT_TYPE(double)},
    {AMetaType::String,     DECLARE_BUILT_TYPE(string)},
    {AMetaType::VariantMap, DECLARE_BUILT_TYPE(AVariantMap)},
    {AMetaType::VariantList,DECLARE_BUILT_TYPE(AVariantList)},
    {AMetaType::ByteArray,  DECLARE_BUILT_TYPE(AByteArray)},
    {AMetaType::Vector2D,   DECLARE_BUILT_TYPE(AVector2D)},
    {AMetaType::Vector3D,   DECLARE_BUILT_TYPE(AVector3D)},
    {AMetaType::Vector4D,   DECLARE_BUILT_TYPE(AVector4D)},
    {AMetaType::Quaternion, DECLARE_BUILT_TYPE(AQuaternion)},
    {AMetaType::Matrix3D,   DECLARE_BUILT_TYPE(AMatrix3D)},
    {AMetaType::Matrix4D,   DECLARE_BUILT_TYPE(AMatrix4D)}
};

static ConverterMap s_Converters= {
    {AMetaType::Bool,      {{AMetaType::Int,        &AMetaType::toBoolean},
                            {AMetaType::Double,     &AMetaType::toBoolean},
                            {AMetaType::String,     &AMetaType::toBoolean}}},

    {AMetaType::Int,       {{AMetaType::Bool,       &AMetaType::toInteger},
                            {AMetaType::Double,     &AMetaType::toInteger},
                            {AMetaType::String,     &AMetaType::toInteger}}},

    {AMetaType::Double,    {{AMetaType::Bool,       &AMetaType::toDouble},
                            {AMetaType::Int,        &AMetaType::toDouble},
                            {AMetaType::String,     &AMetaType::toDouble}}},

    {AMetaType::String,    {{AMetaType::Bool,       &AMetaType::toString},
                            {AMetaType::Int,        &AMetaType::toString},
                            {AMetaType::Double,     &AMetaType::toString}}},

    {AMetaType::VariantList,{{AMetaType::Vector2D,  &AMetaType::toList},
                             {AMetaType::Vector3D,  &AMetaType::toList},
                             {AMetaType::Vector4D,  &AMetaType::toList},
                             {AMetaType::Quaternion,&AMetaType::toList},
                             {AMetaType::Matrix3D,  &AMetaType::toList},
                             {AMetaType::Matrix4D,  &AMetaType::toList}}},

    {AMetaType::Vector2D,   {{AMetaType::Int,       &AMetaType::toVector2D},
                             {AMetaType::Double,    &AMetaType::toVector2D},
                             {AMetaType::VariantList,&AMetaType::toVector2D}}},

    {AMetaType::Vector3D,   {{AMetaType::Int,       &AMetaType::toVector3D},
                             {AMetaType::Double,    &AMetaType::toVector3D},
                             {AMetaType::VariantList,&AMetaType::toVector3D},
                             {AMetaType::Vector2D,  &AMetaType::toVector3D}}},

    {AMetaType::Vector4D,   {{AMetaType::Int,       &AMetaType::toVector4D},
                             {AMetaType::Double,    &AMetaType::toVector4D},
                             {AMetaType::VariantList,&AMetaType::toVector4D},
                             {AMetaType::Vector2D,  &AMetaType::toVector4D},
                             {AMetaType::Vector3D,  &AMetaType::toVector4D}}},

    {AMetaType::Quaternion, {{AMetaType::VariantList,&AMetaType::toQuaternion},
                             {AMetaType::Vector3D,  &AMetaType::toQuaternion}}},

    {AMetaType::Matrix3D,   {{AMetaType::VariantList,&AMetaType::toMatrix3D}}},

    {AMetaType::Matrix4D,   {{AMetaType::VariantList,&AMetaType::toMatrix4D}}}
};

static NameMap s_Names = {
    {"bool",            AMetaType::Bool},
    {"int",             AMetaType::Int},
    {"double",          AMetaType::Double},
    {"string",          AMetaType::String},
    {"map",             AMetaType::VariantMap},
    {"list",            AMetaType::VariantList},
    {"AByteArray",      AMetaType::ByteArray},
    {"AVector2D",       AMetaType::Vector2D},
    {"AVector3D",       AMetaType::Vector3D},
    {"AVector4D",       AMetaType::Vector4D},
    {"AQuaternion",     AMetaType::Quaternion},
    {"AMatrix3D",       AMetaType::Matrix3D},
    {"AMatrix4D",       AMetaType::Matrix4D}
};

AMetaType::AMetaType(const Table *table) :
        m_pTable(table) {
    PROFILE_FUNCTION()
}

const char *AMetaType::name() const {
    PROFILE_FUNCTION()
    return m_pTable->name;
}

int AMetaType::size() const {
    PROFILE_FUNCTION()
    return m_pTable->get_size();
}

void *AMetaType::construct(void *where, const void *copy) const {
    PROFILE_FUNCTION()
    if(copy) {
        m_pTable->clone(&copy, &where);
        return where;
    } else {
        m_pTable->construct(&where);
        return where;
    }
}

void *AMetaType::create(const void *copy) const {
    PROFILE_FUNCTION()
    void *where = nullptr;
    m_pTable->static_new(&where);

    if(copy) {
        m_pTable->clone(&copy, &where);
    }
    return where;
}

void AMetaType::destroy(void *data) const {
    PROFILE_FUNCTION()
    m_pTable->static_delete(&data);
}

void AMetaType::destruct(void *data) const {
    PROFILE_FUNCTION()
    m_pTable->destruct(&data);
}

bool AMetaType::compare(const void *left, const void *right) const {
    PROFILE_FUNCTION()
    return m_pTable->compare(&left, &right);
}

bool AMetaType::isValid() const {
    PROFILE_FUNCTION()
    return (m_pTable != nullptr);
}

uint32_t AMetaType::registerType(Table &table) {
    PROFILE_FUNCTION()
    uint32_t result = ++AMetaType::s_NextId;
    s_Types[result] = table;
    s_Names[table.name] = result;
    return result;
}

uint32_t AMetaType::type(const char *name) {
    PROFILE_FUNCTION()
    auto it = s_Names.find(name);
    if(it != s_Names.end()) {
        return it->second;
    }
    return Invalid;
}
uint32_t AMetaType::type(const type_info &type) {
    PROFILE_FUNCTION()
    for(auto it : s_Types) {
        if(it.second.index() == type_index(type) ) {
            return it.first;
        }
    }
    return Invalid;
}

const char *AMetaType::name(uint32_t type) {
    PROFILE_FUNCTION()
    auto it = s_Types.find(type);
    if(it != s_Types.end()) {
        return AMetaType(&(it->second)).name();
    }
    return nullptr;
}

int AMetaType::size(uint32_t type) {
    PROFILE_FUNCTION()
    auto it = s_Types.find(type);
    if(it != s_Types.end()) {
        return AMetaType(&(it->second)).size();
    }
    return 0;
}

void *AMetaType::construct(uint32_t type, void *where, const void *copy) {
    PROFILE_FUNCTION()
    auto it = s_Types.find(type);
    if(it != s_Types.end()) {
        return AMetaType(&(it->second)).construct(where, copy);
    }
    return nullptr;
}

void *AMetaType::create(uint32_t type, const void *copy) {
    PROFILE_FUNCTION()
    auto it = s_Types.find(type);
    if(it != s_Types.end()) {
        return AMetaType(&(it->second)).create(copy);
    }
    return nullptr;
}

void AMetaType::destroy(uint32_t type, void *data) {
    PROFILE_FUNCTION()
    auto it = s_Types.find(type);
    if(it != s_Types.end()) {
        AMetaType(&(it->second)).destroy(data);
    }
}

void AMetaType::destruct(uint32_t type, void *data) {
    PROFILE_FUNCTION()
    auto it = s_Types.find(type);
    if(it != s_Types.end()) {
        AMetaType(&(it->second)).destruct(data);
    }
}

bool AMetaType::compare(const void *left, const void *right, uint32_t type) {
    PROFILE_FUNCTION()
    auto it = s_Types.find(type);
    if(it != s_Types.end()) {
        return AMetaType(&(it->second)).compare(left, right);
    }
    return false;
}

bool AMetaType::convert(const void *from, uint32_t fromType, void *to, uint32_t toType) {
    PROFILE_FUNCTION()
    auto t = s_Converters.find(toType);
    if(t != s_Converters.end()) {
        auto it = t->second.find(fromType);
        if(it != t->second.end()) {
            return (*it->second)(to, from, fromType);
        }
    }
    return false;
}

bool AMetaType::registerConverter(uint32_t from, uint32_t to, converterCallback function) {
    PROFILE_FUNCTION()
    if(hasConverter(from, to)) {
        return false;
    }

    auto t = s_Converters.find(to);
    if(t != s_Converters.end()) {
        t->second[from] = function;
    } else {
        map<uint32_t, converterCallback> m;
        m[from] = function;
        s_Converters[to]    = m;
    }
    return true;
}

bool AMetaType::hasConverter(uint32_t from, uint32_t to) {
    PROFILE_FUNCTION()
    auto t = s_Converters.find(to);
    if(t != s_Converters.end()) {
        auto it = t->second.find(from);
        if(it != t->second.end()) {
            return true;
        }
    }
    return false;
}

bool AMetaType::toBoolean(void *to, const void *from, const uint32_t fromType) {
    PROFILE_FUNCTION()
    bool result = true;
    bool *r     = static_cast<bool *>(to);
    switch(fromType) {
        case Int:   { *r        = *(static_cast<const int *>(from)) != 0; } break;
        case Double:{ *r        = *(static_cast<const double *>(from)) != 0; } break;
        case String:{
            string s  = *(static_cast<const string *>(from));
            *r = (s != "false" || s != "0" || !s.empty());
        }  break;
        default:    { result    = false; } break;
    }
    return result;
}

bool AMetaType::toInteger(void *to, const void *from, const uint32_t fromType) {
    PROFILE_FUNCTION()
    bool result = true;
    int *r      = static_cast<int *>(to);
    switch(fromType) {
        case Bool:  { *r        = (*(static_cast<const bool *>(from))) ? 1 : 0; } break;
        case Double:{ double f  = *(static_cast<const double *>(from)); *r = int(f); f -= *r; *r += (f >= 0.5f) ? 1 : 0; } break;
        case String:{
            string s  = *(static_cast<const string *>(from));
            *r        = stoi(s);
        } break;
        default:    { result    = false; } break;
    }
    return result;
}

bool AMetaType::toDouble(void *to, const void *from, const uint32_t fromType) {
    PROFILE_FUNCTION()
    bool result = true;
    double *r   = static_cast<double *>(to);
    switch(fromType) {
        case Bool:  { *r        = double(*(static_cast<const bool *>(from))); } break;
        case Int:   { *r        = double(*(static_cast<const int *>(from))); } break;
        case String:{ *r        = stod(*(static_cast<const string *>(from))); } break;
        default:    { result    = false; } break;
    }
    return result;
}

bool AMetaType::toString(void *to, const void *from, const uint32_t fromType) {
    PROFILE_FUNCTION()
    bool result = true;
    string *r   = static_cast<string *>(to);
    switch(fromType) {
        case Bool:  { *r        = (*(static_cast<const bool *>(from))) ? "true" : "false"; } break;
        case Double:{ string s  = to_string(*(static_cast<const double *>(from))); *r = s; } break;
        case Int:   { *r        = to_string(*(static_cast<const int *>(from))); } break;
        default:    { result    = false; } break;
    }
    return result;
}

bool AMetaType::toList(void *to, const void *from, const uint32_t fromType) {
    PROFILE_FUNCTION()
    bool result = true;
    AVariantList *r = static_cast<AVariantList *>(to);
    switch(fromType) {
        case Vector2D: {
            const AVector2D v = *(reinterpret_cast<const AVector2D *>(from));
            for(int i = 0; i < 2; i++) {
                r->push_back(v[i]);
            }
        } break;
        case Vector3D: {
            const AVector3D v = *(reinterpret_cast<const AVector3D *>(from));
            for(int i = 0; i < 3; i++) {
                r->push_back(v[i]);
            }
        } break;
        case Vector4D: {
            const AVector4D v = *(reinterpret_cast<const AVector4D *>(from));
            for(int i = 0; i < 4; i++) {
                r->push_back(v[i]);
            }
        } break;
        case Quaternion: {
            const AQuaternion v = *(reinterpret_cast<const AQuaternion *>(from));
            for(int i = 0; i < 4; i++) {
                r->push_back(v[i]);
            }
        } break;
        case Matrix3D: {
            const AMatrix3D v = *(reinterpret_cast<const AMatrix3D *>(from));
            for(int i = 0; i < 9; i++) {
                r->push_back(v[i]);
            }
        } break;
        case Matrix4D:{
            const AMatrix4D v = *(reinterpret_cast<const AMatrix4D *>(from));
            for(int i = 0; i < 16; i++) {
                r->push_back(v[i]);
            }
        } break;
        default:    { result    = false; } break;
    }
    return result;
}

bool AMetaType::toVector2D(void *to, const void *from, const uint32_t fromType) {
    PROFILE_FUNCTION()
    bool result     = true;
    AVector2D *r    = static_cast<AVector2D *>(to);
    switch(fromType) {
        case Int:   { *r  = AVector2D(*(static_cast<const int *>(from))); } break;
        case Double:{ *r  = AVector2D(*(static_cast<const double *>(from))); } break;
        case VariantList: {
            const AVariantList *list    = reinterpret_cast<const AVariantList *>(from);
            auto it = list->begin();
            for(int i = 0; i < 2; i++, it++) {
                (*r)[i] = (*it).toDouble();
            }
        } break;
        default:    { result    = false; } break;
    }
    return result;
}

bool AMetaType::toVector3D(void *to, const void *from, const uint32_t fromType) {
    PROFILE_FUNCTION()
    bool result     = true;
    AVector3D *r    = static_cast<AVector3D *>(to);
    switch(fromType) {
        case Int:   { *r  = AVector3D(*(static_cast<const int *>(from))); } break;
        case Double:{ *r  = AVector3D(*(static_cast<const double *>(from))); } break;
        case VariantList: {
            const AVariantList *list    = reinterpret_cast<const AVariantList *>(from);
            auto it = list->begin();
            for(int i = 0; i < 3; i++, it++) {
                (*r)[i] = (*it).toDouble();
            }
        } break;
        case Vector2D:  { *r = AVector3D(*(static_cast<const AVector2D *>(from)), 0.0); } break;
        default:    { result    = false; } break;
    }
    return result;
}

bool AMetaType::toVector4D(void *to, const void *from, const uint32_t fromType) {
    PROFILE_FUNCTION()
    bool result     = true;
    AVector4D *r    = static_cast<AVector4D *>(to);
    switch(fromType) {
        case Int:   { *r  = AVector4D(*(static_cast<const int *>(from))); } break;
        case Double:{ *r  = AVector4D(*(static_cast<const double *>(from))); } break;
        case VariantList: {
            const AVariantList *list    = reinterpret_cast<const AVariantList *>(from);
            auto it = list->begin();
            for(int i = 0; i < 4; i++, it++) {
                (*r)[i] = (*it).toDouble();
            }
        } break;
        case Vector2D:  { *r = AVector4D(*(static_cast<const AVector2D *>(from)), 0.0, 1.0); } break;
        case Vector3D:  { *r = AVector4D(*(static_cast<const AVector3D *>(from)), 1.0); } break;
        default:    { result    = false; } break;
    }
    return result;
}

bool AMetaType::toMatrix3D(void *to, const void *from, const uint32_t fromType) {
    PROFILE_FUNCTION()
    bool result     = true;
    AMatrix3D *r    = static_cast<AMatrix3D *>(to);
    switch(fromType) {
        case VariantList: {
            const AVariantList *list    = reinterpret_cast<const AVariantList *>(from);
            auto it = list->begin();
            for(int i = 0; i < 9; i++, it++) {
                (*r)[i]   = (*it).toDouble();
            }
        } break;
        default: { result   = false; } break;
    }
    return result;
}

bool AMetaType::toMatrix4D(void *to, const void *from, const uint32_t fromType) {
    PROFILE_FUNCTION()
    bool result     = true;
    AMatrix4D *r    = static_cast<AMatrix4D *>(to);
    switch(fromType) {
        case VariantList: {
            const AVariantList *list    = reinterpret_cast<const AVariantList *>(from);
            auto it = list->begin();
            for(int i = 0; i < 16; i++, it++) {
                (*r)[i] = (*it).toDouble();
            }
        } break;
        default: { result   = false; } break;
    }
    return result;
}

bool AMetaType::toQuaternion(void *to, const void *from, const uint32_t fromType) {
    PROFILE_FUNCTION()
    bool result     = true;
    AQuaternion *r  = static_cast<AQuaternion *>(to);
    switch(fromType) {
        case VariantList: {
            const AVariantList *list  = reinterpret_cast<const AVariantList *>(from);
            auto it = list->begin();
            for(int i = 0; i < 4; i++, it++) {
                (*r)[i] = (*it).toDouble();
            }
        } break;
        case Vector3D:  { AMatrix3D m; m.rotate(*(static_cast<const AVector3D *>(from))); *r = AQuaternion(m); } break;
        default:    { result    = false; } break;
    }
    return result;
}
