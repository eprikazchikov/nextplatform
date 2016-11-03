#include "aobject.h"

#include "auri.h"

#include "atools.h"

#define TYPE        "Type"
#define NAME        "Name"
#define DATA        "Data"
#define GROUP       "Group"
#define FLAGS       "Flags"
#define ORDER       "Order"
#define LINKS       "Links"
#define ENABLE      "Enable"
#define SLOTS       "Slots"
#define SIGNALS     "Signals"
#define PROTOTYPE   "Prototype"
#define PROPERTIES  "Properties"
#define COMPONENTS  "Components"

AObject::AObject() {
    m_bEnable       = true;
    m_bDelete       = false;
    m_bNative       = false;
    m_pSystem       = 0;
    m_pParent       = 0;
    m_pPrototype    = 0;
    m_sName         = "";
    m_sType         = "";
}

AObject::~AObject() {
    onDestroyed();

    for(const auto &it : m_lModels) {
        delete it;
    }

    for(const auto &it : m_lLinks) {
        if(it.receiver == this) {
            it.sender->removeLink(it);
        }

        if(it.sender == this) {
            it.receiver->removeLink(it);
        }
    }
    m_lLinks.clear();

    for(const auto &it : m_mComponents) {
        if(it.second) {
            it.second->m_pParent    = 0;
            delete it.second;
        }
    }
    m_mComponents.clear();

    if(m_pParent) {
        const auto it   = m_pParent->m_mComponents.find(m_sName);
        if(it != m_pParent->m_mComponents.end()) {
            m_pParent->m_mComponents.erase(it);
        }
    }

    if(m_pPrototype) {
        m_pPrototype->removeModel(this);
    }
}

AObjectSystem *AObject::system() const {
    return m_pSystem;
}

AObject *AObject::parent() const {
    return m_pParent;
}

AObject *AObject::component(const string &name) {
    auto it = m_mComponents.find(name);
    if(it != m_mComponents.end()) {
        return (*it).second;
    }
    return 0;
}

string AObject::name() const {
    return m_sName;
}

void AObject::addModel(AObject *model) {
    m_lModels.push_back(model);
    model->setPrototype(this);
}

void AObject::removeModel(const AObject *model) {
    for(auto it = m_lModels.begin(); it != m_lModels.end(); it++) {
        if(*it == model) {
            m_lModels.erase(it);
            return;
        }
    }
}

void AObject::setPrototype(AObject *prototype) {
    m_pPrototype    = prototype;
}

AVariant AObject::linksToVariant(const string &name) const {
    AVariant result(AVariant::LIST);
    for(const auto &it : m_lLinks) {
        if(it.sender == this && it.signal == name) {
            result.appendProperty(it.receiver->reference() + "?" + it.slot, it.signal);
        } else if(it.receiver == this && it.slot == name) {
            result.appendProperty(it.sender->reference() + "#" + it.signal, it.slot);
        }
    }
    return result;
}

AVariant AObject::propertyToVariant(const string &name) {
    AVariant result(AVariant::MAP);

    if(isPropertyExist(name)) {
        AProperty p = propertySettings(name);

        result.appendProperty(name,     NAME);
        result.appendProperty(p.flags,  FLAGS);
        result.appendProperty(p.group,  GROUP);
        result.appendProperty(p.data,   DATA);
        result.appendProperty(p.type,   TYPE);
        result.appendProperty(p.order,  ORDER);
        result.appendProperty(linksToVariant(name), LINKS);
    }

    return result;
}

AVariant AObject::slotToVariant(const string &name) {
    AVariant result(AVariant::MAP);

    if(isSlotExist(name)) {
        result.appendProperty(name, NAME);
        result.appendProperty(linksToVariant(name), LINKS);
    }

    return result;
}

void AObject::addEventListner(const string &name, const string &reference) {
    AUri uri(reference);
    AObject *o  = findObject(uri.path());
    if(o) {
        if(uri.fragment().empty()) {
            addEventListner(this, name, o, uri.query());
        } else {
            addEventListner(o, uri.fragment(), this, name);
        }
    }
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
    for(auto &it : m_lModels) {
        it->addLink(link);
    }

    /// \todo: Place for link_mutex
    if(!isLinkExist(link)) {
        m_lLinks.push_back(link);
    }
}

void AObject::removeLink(const link_data &link) {
    for(auto &it : m_lModels) {
        it->removeLink(link);
    }

    for(auto it = m_lLinks.begin(); it != m_lLinks.end(); it++) {
        link_data *data = &(*it);
        if(data->sender == link.sender && data->signal == link.signal && data->receiver == link.receiver && data->slot == link.slot) {
            /// \todo: Place for link_mutex
            m_lLinks.erase(it);
            break;
        }
    }
}

void AObject::addSignal(const string &signal) {
    for(auto &it : m_lModels) {
        it->addSignal(signal);
    }

    /// \todo: Place for signal_mutex
    if(!isSignalExist(signal)) {
        m_mSignals.push_back(signal);
    }
}

void AObject::removeSignal(const string &signal) {
    for(auto &it : m_lModels) {
        it->removeSignal(signal);
    }

    for(auto it = m_mSignals.begin(); it != m_mSignals.end(); it++) {
        if(*it == signal) {
            /// \todo: Place for signal_mutex
            m_mSignals.erase(it);
            break;
        }
    }
}

void AObject::addSlot(const string &slot, callback ptr) {
    for(auto &it : m_lModels) {
        it->addSlot(slot, ptr);
    }

    /// \todo: Place for slot_mutex
    if(!isSignalExist(slot)) {
        m_mSlots[slot]  = ptr;
    }
}

void AObject::removeSlot(const string &slot) {
    for(auto &it : m_lModels) {
        it->removeSlot(slot);
    }

    auto it = m_mSlots.find(slot);
    if(it != m_mSlots.end()) {
        /// \todo: Place for slot_mutex
        m_mSlots.erase(it);
    }
}

void AObject::deleteLater() {
    m_bDelete   = true;
}

AObject::objectsMap &AObject::getComponents() {
    return m_mComponents;
}

AObject::propertiesMap &AObject::getProperties() {
    return m_mProperties;
}

const AObject::stringVector &AObject::getSignals() {
    return m_mSignals;
}

AObject::slotsMap &AObject::getSlots() {
    return m_mSlots;
}

AObject::linksList &AObject::getLinks() {
    return m_lLinks;
}

AObject *AObject::findObject(const string &path) {
    if(m_pParent && path[0] == '/') {
        return m_pParent->findObject(path);
    }

    unsigned int start  = 0;
    if(path[0] == '/') {
        start   = 1;
    }
    int index  = path.find('/', 1);
    if(index > -1) {
        for(const auto &it : m_mComponents) {
            AObject *o  = it.second->findObject(path.substr(index + 1));
            if(o) {
                return o;
            }
        }
    } else if(path.substr(start, index) == m_sName) {
        return this;
    }

    return 0;
}

void AObject::setSystem(AObjectSystem *system) {
    m_pSystem   = system;
}

void AObject::setParent(AObject *parent) {
    if(parent) {
        parent->addComponent(m_sName, this);
    }
    m_pParent   = parent;
}

void AObject::setName(const string &value) {
    if(!value.empty()) {
        if(m_sName != value && m_pParent) {
            /// \todo: Remove Component
            auto it = m_pParent->m_mComponents.find(m_sName);
            if(it !=  m_pParent->m_mComponents.end()) {
                m_pParent->m_mComponents.erase(it);
            }

            m_pParent->addComponent(value, this);
        }
        m_sName = value;
    }
}

void AObject::setType(const string &value) {
    m_sType = value;
}

void AObject::addComponent(const string &name, AObject *value) {
    if(value) {
        string type = name;
        if(type.empty()) {
            type    = value->typeName();
        }
        string str  = type;
        int i       = 0;
        while(true) {
            if(m_mComponents.find(str) == m_mComponents.end()) {
                break;
            } else {
                i++;
                str = type + "_" + to_string(i);
            }
        }

        m_mComponents[str]  = value;

        value->m_pParent    = this;
        value->m_sName      = str;
    }
}

bool AObject::isEnable() const {
    return m_bEnable;
}

bool AObject::isNative() const {
    return m_bNative;
}

bool AObject::isLinkExist(const link_data &link) const {
    for(const auto &it : m_lLinks) {
        if(it.signal == link.signal && it.slot == link.slot &&
           it.sender == link.sender && it.receiver == link.receiver) {
            return true;
        }
    }
    return false;
}

bool AObject::isPropertyExist(const string &property) const {
    return (m_mProperties.find(property) != m_mProperties.end());
}

bool AObject::isSignalExist(const string &signal) const {
    for(const auto &it : m_mSignals) {
        if(it == signal) {
            return true;
        }
    }
    return false;
}

bool AObject::isSlotExist(const string &slot) const {
    return (m_mSlots.find(slot) != m_mSlots.end());
}

bool AObject::update() {
    auto it = m_mComponents.begin();
    while(it != m_mComponents.end()) {
        AObject *c  = it->second;
        if(c && c->update()) {
            it  = m_mComponents.erase(it);
            c->setParent(0);
            delete c;
            continue;
        }
        it++;
    }

    return m_bDelete;
}

void AObject::dispatchEvent(const string &name, const AVariant &args) {
    if(m_mProperties.find(name) != m_mProperties.end()) {
        setProperty(name, args);
    } else {
        if(m_mSlots.find(name) != m_mSlots.end()) {
            callback call   = m_mSlots[name];
            if(call) {
                (*call)(this, args);
            } else {
                onEvent(name, args);
            }
        }
    }
}

void AObject::onEvent(const string &, const AVariant &) {

}

void AObject::emitSignal(const string &name, const AVariant &args) {
    // Notify connected links
    for(auto &it : m_lLinks) {
        link_data *link     = &it;
        if(link->sender == this && link->signal == name) {
            link->receiver->dispatchEvent(link->slot, args);
        }
    }
}

AVariant AObject::property(const string &name) {
    return m_mProperties[name].data;
}

string AObject::reference() const {
    if(m_pParent) {
        return m_pParent->reference() + "/" + m_sName;
    }
    return string("thor://") + ((m_pSystem) ? m_pSystem->systemName() : "system") + "/" + m_sName;
}

AObject *AObject::createInstance() {
    AObject *result = objectFactory();
    if(result) {
        result->setSystem(m_pSystem);
        addModel(result);
        *result = *this;

        result->onCreated();
    }
    return result;
}

void AObject::setEnable(bool state) {
    m_bEnable   = state;
}

void AObject::setProperty(const string &name, const AVariant &value) {
    AProperty &left = m_mProperties[name];
    for(auto &it : m_lModels) {
        AProperty &right    = it->m_mProperties[name];
        if(left == right) {
            it->setProperty(name, value);
        }
    }
    left.data = value;

    emitSignal(name, value);
}

AProperty AObject::propertySettings(const string &name) {
    return m_mProperties[name];
}

void AObject::setPropertySettings(const string &name, const int flags, const string &group, const string &type, const int order) {
    auto &it    = m_mProperties[name];
    if(!(it.flags & AProperty::NATIVE)) {
        it.flags    = (AProperty::AccessTypes)flags;
        it.group    = group;
        it.type     = type;
        it.order    = order;
    }
}

void AObject::onCreated() {

}

void AObject::onDestroyed() {

}

AVariant AObject::toVariant() {
    AVariant result(AVariant::MAP);

    result.appendProperty(typeName(),   TYPE);

    result.appendProperty(m_sName,      NAME);
    result.appendProperty(m_bEnable,    ENABLE);

    {
        AVariant s(AVariant::LIST);
        for(const auto &it : m_mProperties) {
            AVariant left   = propertyToVariant(it.first);
            if(m_pPrototype && left == m_pPrototype->propertyToVariant(it.first)) {
                continue;
            }

            s.appendProperty(left);
        }
        result.appendProperty(s, PROPERTIES);
    }

    {
        AVariant s(AVariant::LIST);
        for(const auto &it : m_mSignals) {
            AVariant left(AVariant::MAP);

            left.appendProperty(it, NAME);
            left.appendProperty(linksToVariant(it), LINKS);

            if(m_pPrototype) {
                AVariant right(AVariant::MAP);
                /// \todo: Should we find this signal at first?
                right.appendProperty(it, NAME);
                right.appendProperty(m_pPrototype->linksToVariant(it), LINKS);

                if(left == right) {
                    continue;
                }
            }

            s.appendProperty(left);
        }
        result.appendProperty(s, SIGNALS);
    }

    {
        AVariant s(AVariant::LIST);
        for(const auto &it : m_mSlots) {
            AVariant left   = slotToVariant(it.first);
            if(m_pPrototype && left == m_pPrototype->slotToVariant(it.first)) {
                continue;
            }

            s.appendProperty(left);
        }
        result.appendProperty(s, SLOTS);
    }

    {
        AVariant components(AVariant::MAP);
        for(const auto &it : m_mComponents) {
            if(it.second) {
                AVariant left   = *it.second->toVariant();
                if(m_pPrototype) {
                    AObject *c  = m_pPrototype->component(it.first);
                    if(c && left == c->toVariant()) {
                        continue;
                    }
                }

                components.appendProperty(left, it.first);
            } else {
                components.appendProperty(static_cast<void *>(NULL), it.first);
            }
        }
        result.appendProperty(components, COMPONENTS);
    }

    return result;
}

void AObject::fromVariant(const AVariant &variant) {
    AVariant::AVariantMap map   = variant.toMap();

    setName(map[NAME].toString());
    setEnable(map[ENABLE].toBool());
    setType(map[TYPE].toString());

    {
        for(const auto &it : map[PROPERTIES].toList()) {
            auto m      = it.toMap();

            setProperty(m[NAME].toString(), m[DATA]);
            setPropertySettings(m[NAME].toString(), m[FLAGS].toInt(), m[GROUP].toString(), m[TYPE].toString(), m[ORDER].toInt());

            for(const auto &link : m[LINKS].toList()) {
                addEventListner(m[NAME].toString(), link.toString());
            }
        }
    }


    {
        for(const auto &it : map[SLOTS].toList()) {
            auto m      = it.toMap();

            addSlot(m[NAME].toString(), NULL);

            for(const auto &link : m[LINKS].toList()) {
                addEventListner(m[NAME].toString(), link.toString());
            }
        }
    }

    {
        for(const auto &it : map[SIGNALS].toList()) {
            auto m      = it.toMap();

            addSignal(m[NAME].toString());

            for(const auto &link : m[LINKS].toList()) {
                addEventListner(m[NAME].toString(), link.toString());
            }
        }
    }

    {
        for(const auto &it : map[COMPONENTS].toMap()) {
            if(m_mComponents.find(it.first) == m_mComponents.end()) {
                toObject(it.second, m_pSystem, this);
            }
        }
    }
}

AObject *AObject::toObject(const AVariant &variant, AObjectSystem *system, AObject *parent) {
    AVariant::AVariantMap map   = variant.toMap();

    AObject *result = system->objectCreate(map[TYPE].toString(), parent);
    if(result) {
        result->fromVariant(variant);
    }
    return result;
}

AObject &AObject::operator=(AObject &right) {
    fromVariant(right.toVariant());
    return *this;
}

bool AObject::operator==(const AObject &right) {
    bool result = true;

    result &= m_bEnable         == right.m_bEnable;
    result &= m_mSignals        == right.m_mSignals;
    result &= m_mProperties     == right.m_mProperties;
    result &= m_sType           == right.m_sType;

    if(!result) {
        return result;
    }

    {
        if(m_lLinks.size() == right.m_lLinks.size()) {
            for(const auto &li : m_lLinks) {
                result  = false;
                for(const auto &ri : right.m_lLinks) {
                    if(li == ri) {
                        result  = true;
                        break;
                    }
                }
                if(!result) {
                    return result;
                }
            }
        } else {
            return false;
        }
    }

    {
        if(m_mSlots.size() == right.m_mSlots.size()) {
            for(const auto &it : m_mSlots) {
                if(right.m_mSlots.find(it.first) == right.m_mSlots.end()) {
                    return false;
                }
            }
        } else {
            return false;
        }
    }
    {
        if(m_mComponents.size()  == right.m_mComponents.size()) {
            for(const auto &it : m_mComponents) {
                const auto &c   = right.m_mComponents.find(it.first);
                if(c != right.m_mComponents.end()) {
                    if(*(it.second) != *(c->second)) {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        } else {
            return false;
        }
    }

    return true;
}

inline bool AObject::operator!=(const AObject &right) {
    return !(*this == right);
}

inline bool operator==(const AProperty &left, const AProperty &right) {
    bool result = true;
    result &= left.flags    == right.flags;
    result &= left.data     == right.data;
    result &= left.type     == right.type;
    return result;
}

inline bool operator!=(const AProperty &left, const AProperty &right) {
    return !(left == right);
}

inline bool operator==(const AObject::link_data &left, const AObject::link_data &right) {
    bool result = true;
    result &= left.signal   == right.signal;
    result &= left.slot     == right.slot;
    return result;
}

inline bool operator!=(const AObject::link_data &left, const AObject::link_data &right) {
    return !(left == right);
}
