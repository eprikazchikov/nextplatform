#include "aobject.h"

#include "aobjectsystem.h"

AObject::AObject(AObject *parent) {
    m_bEnable   = true;
    m_bDelete   = false;

    m_id        = 0;

    m_pParent   = 0;

    setParent(parent);
}

AObject::~AObject() {
    auto it = m_lLinks.begin();
    for(it; it != m_lLinks.end(); it++) {
        link_data data  = (*it);

        if(data.receiver == this) {
            data.sender->removeLink(data);
        }

        if(data.sender == this) {
            data.receiver->removeLink(data);
        }
    }
    m_lLinks.clear();
}

unsigned int AObject::id() const {
    return m_id;
}

AObject *AObject::parent() const {
    return m_pParent;
}

AObject *AObject::component(string &name) {
    return m_mComponents[name];
}

string AObject::name() const {
    return m_sName;
}

void AObject::addEventListner(AObject *sender, const string &signal, AObject *receiver, const string &slot) {
    if(sender && receiver) {
        link_data data;
        data.sender     = sender;
        data.signal     = signal;
        data.receiver   = receiver;
        data.slot       = slot;

        sender->addLink(data);
        receiver->addLink(data);
    }
}

void AObject::removeEventListner(AObject *sender, const string &signal, AObject *receiver, const string &slot) {
    if(sender && receiver) {
        link_data data;
        data.sender     = sender;
        data.signal     = signal;
        data.receiver   = receiver;
        data.slot       = slot;

        sender->removeLink(data);
        receiver->removeLink(data);
    }
}

void AObject::addLink(link_data &link) {
    /// \todo: Place for link_mutex
    m_lLinks.push_back(link);
}

void AObject::removeLink(link_data &link) {
    auto it = m_lLinks.begin();
    for(it; it != m_lLinks.end(); it++) {
        link_data *data = &(*it);
        if(data->sender == link.sender && data->signal == link.signal && data->receiver == link.receiver && data->slot == link.slot) {
            /// \todo: Place for link_mutex
            m_lLinks.erase(it);
            break;
        }
    }
}

void AObject::deleteLater() {
    m_bDelete   = true;
}

AObject::objects_map AObject::getComponents() const {
    return m_mComponents;
}

AObject::properties_map AObject::getProperties() const {
    return m_mProperties;
}

AObject::string_vector AObject::getSignals() const {
    return m_mSignals;
}

AObject::slots_map AObject::getSlots() const {
    return m_mSlots;
}

AObject::links_list AObject::getLinks() const {
    return m_lLinks;
}

void AObject::setId(unsigned int id) {
    m_id        = id;
}

void AObject::setParent(AObject *parent) {
    if(parent)
        parent->setComponent(m_sName, this);
}

void AObject::setName(const string &value) {
    if(m_sName != value && m_pParent) {
        m_pParent->setComponent(value, this);

        auto it = m_pParent->m_mComponents.find(m_sName);
        if(it != m_pParent->m_mComponents.end()) {
            m_pParent->m_mComponents.erase(it);
        }
    }
    m_sName = value;

}

void AObject::setComponent(const string &name, AObject *value) {
    if(m_mComponents[name])
        delete m_mComponents[name];

    m_mComponents[name]     = value;
    if(value)
        value->m_pParent    = this;
}

bool AObject::isEnable() const {
    return m_bEnable;
}

AObject *AObject::callbackClassFactory(AObject *parent) {
    return new AObject(parent);
}

void AObject::registerClassFactory() {
    AObject object;
    AObjectSystem::instance()->factoryAdd(object.reference(), &AObject::callbackClassFactory);
}


bool AObject::update(float dt) {
    auto it = m_mComponents.begin();
    for(it; it != m_mComponents.end(); it++) {
        if(it->second) {
            it->second->update(dt);
        }
    }

    return m_bDelete;
}

void AObject::dispatchEvent(const string &name, const variant_vector &args) {
    if(m_mProperties.find(name) != m_mProperties.end()) {
        setProperty(name, args.front());
    } else {
        if(m_mSlots.find(name) != m_mSlots.end()) {
            (*m_mSlots[name])(this, args);
        } else {
            AObject *obj    = m_mComponents[name];
            if(obj)
                obj->dispatchEvent(name, args);
        }
    }
}

void AObject::emitSignal(const string &name, const variant_vector &args) {
    // Notify connected links
    auto it = m_lLinks.begin();
    for(it; it != m_lLinks.end(); it++) {
        link_data *link     = &(*it);
        if(link->sender == this && link->signal == name) {
            link->receiver->dispatchEvent(link->slot, args);
        }
    }
}

AVariant AObject::property(const string &name) {
    return m_mProperties[name].data;
}

string AObject::reference() const {
    return "thor://factory/AObject";
}

string AObject::typeName() const {
    return "AObject";
}

void AObject::setEnable(bool state) {
    m_bEnable   = state;
}

void AObject::setProperty(const string &name, const AVariant &value) {
    m_mProperties[name].data    = value;

    emitSignal(name, variant_vector(1, value));
}

void AObject::setPropertyFlags(const string &name, const int flags) {
    m_mProperties[name].flags   = (AccessTypes)flags;
}
