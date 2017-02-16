#include "avariant.h"

#include "atools.h"

AVariant::Data::Data() {
    type    = AMetaType::Invalid;
    so      = nullptr;
}

AVariant::AVariant() {

}

AVariant::AVariant(AMetaType::Type type) {
    mData.type      = type;
}

AVariant::AVariant(bool value) {
    *this   = fromValue<bool>(value);
}

AVariant::AVariant(int value) {
    *this   = fromValue<int>(value);
}

AVariant::AVariant(double value) {
    *this   = fromValue<double>(value);
}

AVariant::AVariant(const char *value) {
    *this   = fromValue<string>(value);
}

AVariant::AVariant(const string &value) {
    *this   = fromValue<string>(value);
}

AVariant::AVariant(const AVariantMap &value) {
    *this   = fromValue<AVariantMap>(value);
}

AVariant::AVariant(const AVariantList &value) {
    *this   = fromValue<AVariantList>(value);
}

AVariant::AVariant(const AVector2D &value) {
    *this   = fromValue<AVector2D>(value);
}

AVariant::AVariant(const AVector3D &value) {
    *this   = fromValue<AVector3D>(value);
}

AVariant::AVariant(const AVector4D &value) {
    *this   = fromValue<AVector4D>(value);
}

AVariant::AVariant(const AQuaternion &value) {
    *this   = fromValue<AQuaternion>(value);
}

AVariant::AVariant(const AMatrix3D &value) {
    *this   = fromValue<AMatrix3D>(value);
}

AVariant::AVariant(const AMatrix4D &value) {
    *this   = fromValue<AMatrix4D>(value);
}

AVariant::AVariant(const ACurve &value) {
    *this   = fromValue<ACurve>(value);
}

AVariant::AVariant(uint32_t type, void *copy) {
    mData.type  = type;
    mData.so    = AMetaType::create(type, copy);
}

AVariant::~AVariant() {
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
    return mData.type;
}

void *AVariant::data() const {
    return mData.so;
}

bool AVariant::canConvert(uint32_t type) const {
    return AMetaType::hasConverter(mData.type, type);
}

// Conversion and getters
bool AVariant::toBool() const {
    return value<bool>();
}

int AVariant::toInt() const {
    return value<int>();
}

double AVariant::toDouble() const {
    return value<double>();
}

const string AVariant::toString() const {
    return value<string>();
}

const AVariant::AVariantMap AVariant::toMap() const {
    AVariantMap result  = value<AVariantMap>();

    if(mData.type > AMetaType::VariantList) {
        result[STRUCTURE]   = static_cast<int>(mData.type);
        result[DATA]        = toList();
    }
    return result;
}

const AVariant::AVariantList AVariant::toList() const {
    return value<AVariantList>();
}

const AVector2D AVariant::toVector2D() const {
    return value<AVector2D>();
}

const AVector3D AVariant::toVector3D() const {
    return value<AVector3D>();
}

const AVector4D AVariant::toVector4D() const {
    return value<AVector4D>();
}

const AQuaternion AVariant::toQuaternion() const {
    return value<AQuaternion>();
}

const AMatrix3D AVariant::toMatrix3D() const {
    return value<AMatrix3D>();
}

const AMatrix4D AVariant::toMatrix4D() const {
    return value<AMatrix4D>();
}

const ACurve AVariant::toCurve() const {
    return value<ACurve>();
}
