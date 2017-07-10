#include "avariant.h"

AVariant::Data::Data() {
    PROFILE_FUNCTION()
    type    = AMetaType::Invalid;
    so      = nullptr;
}

AVariant::AVariant() {
    PROFILE_FUNCTION()

}

AVariant::AVariant(AMetaType::Type type) {
    PROFILE_FUNCTION()
    mData.type      = type;
}

AVariant::AVariant(bool value) {
    PROFILE_FUNCTION()
    *this   = fromValue<bool>(value);
}

AVariant::AVariant(int value) {
    PROFILE_FUNCTION()
    *this   = fromValue<int>(value);
}

AVariant::AVariant(double value) {
    PROFILE_FUNCTION()
    *this   = fromValue<double>(value);
}

AVariant::AVariant(const char *value) {
    PROFILE_FUNCTION()
    *this   = fromValue<string>(value);
}

AVariant::AVariant(const string &value) {
    PROFILE_FUNCTION()
    *this   = fromValue<string>(value);
}

AVariant::AVariant(const AVariantMap &value) {
    PROFILE_FUNCTION()
    *this   = fromValue<AVariantMap>(value);
}

AVariant::AVariant(const AVariantList &value) {
    PROFILE_FUNCTION()
    *this   = fromValue<AVariantList>(value);
}

AVariant::AVariant(const AByteArray &value) {
    PROFILE_FUNCTION()
    *this   = fromValue<AByteArray>(value);
}

AVariant::AVariant(const AVector2D &value) {
    PROFILE_FUNCTION()
    *this   = fromValue<AVector2D>(value);
}

AVariant::AVariant(const AVector3D &value) {
    PROFILE_FUNCTION()
    *this   = fromValue<AVector3D>(value);
}

AVariant::AVariant(const AVector4D &value) {
    PROFILE_FUNCTION()
    *this   = fromValue<AVector4D>(value);
}

AVariant::AVariant(const AQuaternion &value) {
    PROFILE_FUNCTION()
    *this   = fromValue<AQuaternion>(value);
}

AVariant::AVariant(const AMatrix3D &value) {
    PROFILE_FUNCTION()
    *this   = fromValue<AMatrix3D>(value);
}

AVariant::AVariant(const AMatrix4D &value) {
    PROFILE_FUNCTION()
    *this   = fromValue<AMatrix4D>(value);
}

AVariant::AVariant(const ACurve &value) {
    PROFILE_FUNCTION()
    *this   = fromValue<ACurve>(value);
}

AVariant::AVariant(uint32_t type, void *copy) {
    PROFILE_FUNCTION()
    mData.type  = type;
    mData.so    = AMetaType::create(type, copy);
}

AVariant::~AVariant() {
    PROFILE_FUNCTION()
}

AVariant &AVariant::operator=(const AVariant &value) {
    mData   = value.mData;
    return *this;
}

bool AVariant::operator==(const AVariant &right) const {
    if(mData.type  == right.mData.type) {
        return AMetaType::compare(mData.so, right.mData.so, mData.type);
    }
    return false;
}

bool AVariant::operator!=(const AVariant &right) const {
    return !(*this == right);
}

uint32_t AVariant::type() const {
    PROFILE_FUNCTION()
    return (mData.type < AMetaType::UserType) ? mData.type : AMetaType::UserType;
}

uint32_t AVariant::userType() const {
    PROFILE_FUNCTION()
    return mData.type;
}

void *AVariant::data() const {
    return mData.so;
}

bool AVariant::isValid() const {
    return (mData.type != AMetaType::Invalid && mData.so);
}

bool AVariant::canConvert(uint32_t type) const {
    PROFILE_FUNCTION()
    return AMetaType::hasConverter(mData.type, type);
}

// Conversion and getters
bool AVariant::toBool() const {
    PROFILE_FUNCTION()
    return value<bool>();
}

int AVariant::toInt() const {
    PROFILE_FUNCTION()
    return value<int>();
}

double AVariant::toDouble() const {
    PROFILE_FUNCTION()
    return value<double>();
}

const string AVariant::toString() const {
    PROFILE_FUNCTION()
    return value<string>();
}

const AVariant::AVariantMap AVariant::toMap() const {
    PROFILE_FUNCTION()
    return value<AVariantMap>();
}

const AVariant::AVariantList AVariant::toList() const {
    PROFILE_FUNCTION()
    AVariantList result = value<AVariantList>();
    result.push_front(static_cast<int>(mData.type));
    return result;
}

const AVariant::AByteArray AVariant::toByteArray() const {
    PROFILE_FUNCTION()
    return value<AByteArray>();
}

const AVector2D AVariant::toVector2D() const {
    PROFILE_FUNCTION()
    return value<AVector2D>();
}

const AVector3D AVariant::toVector3D() const {
    PROFILE_FUNCTION()
    return value<AVector3D>();
}

const AVector4D AVariant::toVector4D() const {
    PROFILE_FUNCTION()
    return value<AVector4D>();
}

const AQuaternion AVariant::toQuaternion() const {
    PROFILE_FUNCTION()
    return value<AQuaternion>();
}

const AMatrix3D AVariant::toMatrix3D() const {
    PROFILE_FUNCTION()
    return value<AMatrix3D>();
}

const AMatrix4D AVariant::toMatrix4D() const {
    PROFILE_FUNCTION()
    return value<AMatrix4D>();
}

const ACurve AVariant::toCurve() const {
    PROFILE_FUNCTION()
    return value<ACurve>();
}
