#include "ametaproperty.h"

AMetaProperty::AMetaProperty(const Table *table) :
        m_pTable(table) {
}

const char *AMetaProperty::name() const {
    return m_pTable->name;
}

bool AMetaProperty::isValid() const {
    return (m_pTable != nullptr);
}

const AMetaType AMetaProperty::type() const {
    return AMetaType(m_pTable->type);
}

AVariant AMetaProperty::read(const AObject *obj) const {
    return m_pTable->reader(obj);
}

void AMetaProperty::write(AObject *obj, const AVariant &value) const {
    m_pTable->writer(obj, value);
}
