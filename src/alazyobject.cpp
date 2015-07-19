#include "alazyobject.h"

ALazyObject::ALazyObject(AObject *prototype) :
        AObject() {
    prototype->addModel(this);

    m_sType         = prototype->typeName();
    m_pPrototype    = prototype;
}

ALazyObject::~ALazyObject() {
    m_pPrototype->removeModel(this);
}

AObject *ALazyObject::prototype() const {
    return m_pPrototype;
}

AVariant ALazyObject::property(const string &name) {
    const auto &it  = m_mProperties.find(name);
    if(it != m_mProperties.end()) {
        return (*it).second.data;
    }
    return m_pPrototype->property(name);
}

AProperty ALazyObject::propertySettings(const string &name) {
    const auto &it  = m_mProperties.find(name);
    if(it != m_mProperties.end()) {
        auto result = (*it).second;
        const auto list = m_pPrototype->getProperties();
        const auto &p   = list.find(name);
        if(p != list.end()) {
            result.flags    = (*p).second.flags;
            result.type     = (*p).second.type;
            result.order    = (*p).second.order;
        }
        return result;
    }
    return m_pPrototype->propertySettings(name);
}

AObject::objectsMap ALazyObject::getComponents() const {
    AObject::objectsMap result  = AObject::getComponents();
    AObject::objectsMap proto   = m_pPrototype->getComponents();
    result.insert(proto.begin(), proto.end());
    return result;
}

AObject::propertiesMap ALazyObject::getProperties() const {
    AObject::propertiesMap result   = AObject::getProperties();
    AObject::propertiesMap proto    = m_pPrototype->getProperties();
    result.insert(proto.begin(), proto.end());
    return result;
}

AObject::stringVector ALazyObject::getSignals() const {
    AObject::stringVector result    = AObject::getSignals();
    AObject::stringVector proto     = m_pPrototype->getSignals();
    result.insert(result.begin(), proto.begin(), proto.end());
    return result;
}

AObject::slotsMap ALazyObject::getSlots() const {
    AObject::slotsMap result    = AObject::getSlots();
    AObject::slotsMap proto     = m_pPrototype->getSlots();
    result.insert(proto.begin(), proto.end());
    return result;
}

AObject::linksList ALazyObject::getLinks() const {
    AObject::linksList result   = AObject::getLinks();
    AObject::linksList proto    = m_pPrototype->getLinks();
    result.insert(result.begin(), proto.begin(), proto.end());
    return result;
}

void ALazyObject::addLink(link_data &link) {
    if(!isLinkExist(link) && !m_pPrototype->isLinkExist(link)) {
        m_lLinks.push_back(link);
    }
}

void ALazyObject::addSignal(const string &signal) {
    if(!isSignalExist(signal) && !m_pPrototype->isSignalExist(signal)) {
        m_mSignals.push_back(signal);
    }
}

void ALazyObject::addSlot(const string &slot, callback ptr) {
    if(!isSlotExist(slot) && !m_pPrototype->isSlotExist(slot)) {
        m_mSlots[slot]  = ptr;
    }
}

AObject *ALazyObject::findObject(const string &path) {
    AObject *result = AObject::findObject(path);
    if(!result) {
        result      = m_pPrototype->findObject(path);
    }
    return result;
}

const string ALazyObject::typeName() const {
    return m_sType;
}
