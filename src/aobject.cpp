#include "aobject.h"

#include "auri.h"

#define TYPE        "Type"
#define NAME        "Name"
#define DATA        "Data"
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
    m_pParent       = 0;
    m_sName         = "";
    m_id            = AObjectSystem::instance()->nextId();
}

AObject::~AObject() {
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

void AObject::addModel(AObject *model) {
    m_lModels.push_back(model);
}

void AObject::removeModel(const AObject *model) {
    for(auto it = m_lModels.begin(); it != m_lModels.end(); it++) {
        if(*it == model) {
            m_lModels.erase(it);
            return;
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
    /// \todo: Place for link_mutex
    if(!isLinkExist(link)) {
        m_lLinks.push_back(link);
    }
}

void AObject::removeLink(const link_data &link) {
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
    /// \todo: Place for signal_mutex
    if(!isSignalExist(signal)) {
        m_mSignals.push_back(signal);
    }
}

void AObject::removeSignal(const string &signal) {
    for(auto it = m_mSignals.begin(); it != m_mSignals.end(); it++) {
        if(*it == signal) {
            /// \todo: Place for signal_mutex
            m_mSignals.erase(it);
            break;
        }
    }
}

void AObject::addSlot(const string &slot, callback ptr) {
    /// \todo: Place for slot_mutex
    if(!isSignalExist(slot)) {
        m_mSlots[slot]  = ptr;
    }
}

void AObject::removeSlot(const string &slot) {
    auto it = m_mSlots.find(slot);
    if(it != m_mSlots.end()) {
        /// \todo: Place for slot_mutex
        m_mSlots.erase(it);
    }
}

void AObject::deleteLater() {
    m_bDelete   = true;
}

AObject::objectsMap AObject::getComponents() const {
    return m_mComponents;
}

AObject::propertiesMap AObject::getProperties() const {
    return m_mProperties;
}

AObject::stringVector AObject::getSignals() const {
    return m_mSignals;
}

AObject::slotsMap AObject::getSlots() const {
    return m_mSlots;
}

AObject::linksList AObject::getLinks() const {
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
    return (m_mSlots.find(property) != m_mSlots.end());
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

bool AObject::update(float dt) {
    auto it = m_mComponents.begin();
    for(it; it != m_mComponents.end(); it++) {
        AObject *c  = it->second;
        if(c && c->update(dt)) {
            delete c;
        }
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

void AObject::onEvent(const string &name, const AVariant &args) {

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
    return string("thor://" + AObjectSystem::instance()->systemName() + "/" + m_sName);
}

void AObject::setEnable(bool state) {
    m_bEnable   = state;
}

void AObject::setProperty(const string &name, const AVariant &value) {
    m_mProperties[name].data = value;

    emitSignal(name, value);
}

AProperty AObject::propertySettings(const string &name) {
    return m_mProperties[name];
}

void AObject::setPropertySettings(const string &name, const int flags, const int type, const int order) {
    auto &it    = m_mProperties[name];
    if(!(it.flags & AProperty::NATIVE)) {
        it.flags    = (AProperty::AccessTypes)flags;
        it.type     = type;
        it.order    = order;
    }
}

AVariant AObject::toVariant() {
    AVariant result(AVariant::MAP);

    result.appendProperty(typeName(),   TYPE);

    result.appendProperty(m_sName,      NAME);
    result.appendProperty(m_bEnable,    ENABLE);

    {
        AVariant s(AVariant::LIST);
        for(const auto &it : m_mProperties) {
            AVariant v(AVariant::MAP);

            AProperty p = propertySettings(it.first);

            v.appendProperty(it.first,  NAME);
            v.appendProperty(p.flags,   FLAGS);
            v.appendProperty(p.data,    DATA);
            v.appendProperty(p.type,    TYPE);
            v.appendProperty(p.order,   ORDER);

            s.appendProperty(v);
        }
        result.appendProperty(s, PROPERTIES);
    }

    {
        AVariant s(AVariant::LIST);
        for(const auto &it : m_mSignals) {
            s.appendProperty(it);
        }
        result.appendProperty(s, SIGNALS);
    }

    {
        AVariant s(AVariant::LIST);
        for(const auto &it : m_mSlots) {
            s.appendProperty(it.first);
        }
        result.appendProperty(s, SLOTS);
    }

    {
        AVariant components(AVariant::MAP);
        for(const auto &it : m_mComponents) {
            if(it.second) {
                components.appendProperty(*it.second->toVariant(), it.first);
            } else {
                components.appendProperty(NULL, it.first);
            }
        }
        result.appendProperty(components, COMPONENTS);
    }

    {
        AVariant s(AVariant::MAP);
        for(const auto &it : m_lLinks) {
            if(it.sender == this) {
                s.appendProperty(it.receiver->reference() + "?" + it.slot, it.signal);
            } else if(it.receiver == this) {
                s.appendProperty(it.sender->reference() + "#" + it.signal, it.slot);
            }
        }
        result.appendProperty(s, LINKS);
    }

    return result;
}

void AObject::fromVariant(const AVariant &variant) {
    AVariant::AVariantMap map   = variant.toMap();

    setName(map[NAME].toString());
    setEnable(map[ENABLE].toBool());

    {
        for(const auto &it : map[PROPERTIES].toList()) {
            AVariant::AVariantMap p = it.toMap();
            setProperty(p[NAME].toString(), p[DATA]);
            setPropertySettings(p[NAME].toString(), p[FLAGS].toInt(), p[TYPE].toInt(), p[ORDER].toInt());
        }
    }


    {
        for(const auto &it : map[SLOTS].toList()) {
            auto m  = it.toMap();
            addSlot(m[NAME].toString(), NULL);
        }
    }

    {
        for(const auto &it : map[SIGNALS].toList()) {
            auto m      = it.toMap();
            addSignal(m[NAME].toString());
        }
    }

    {
        for(const auto &it : map[COMPONENTS].toMap()) {
            if(m_mComponents.find(it.first) == m_mComponents.end()) {
                toObject(it.second, this);
            }
        }
    }

    {
        for(const auto &it : map[LINKS].toMap()) {
            AUri uri(it.second.toString());
            AObject *o  = findObject(uri.path());
            if(o) {
                if(uri.fragment().empty()) {
                    addEventListner(this, it.first, o, uri.query());
                } else {
                    addEventListner(o, uri.fragment(), this, it.first);
                }
            }

        }
    }
}

AObject *AObject::toObject(const AVariant &variant, AObject *parent) {
    AVariant::AVariantMap map   = variant.toMap();

    AObject *result = AObjectSystem::instance()->createObject(map[TYPE].toString(), parent);
    if(result) {
        result->fromVariant(variant);
    }
    return result;
}

AObject &AObject::operator=(AObject &right) {
    fromVariant(right.toVariant());
    return *this;
}

bool AObject::operator==(const AObject &right) const {
    bool result = true;

    result &= m_bEnable         == right.m_bEnable;
    result &= getSignals()      == right.getSignals();
    result &= getProperties()   == right.getProperties();

    if(!result) {
        return result;
    }

    {
        auto l  = getLinks();
        auto r  = right.getLinks();
        if(l.size() == r.size()) {
            for(const auto &li : l) {
                result  = false;
                for(const auto &ri : r) {
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
        auto l  = getSlots();
        auto r  = right.getSlots();
        if(l.size()  == r.size()) {
            for(const auto &it : l) {
                if(r.find(it.first) == r.end()) {
                    return false;
                }
            }
        } else {
            return false;
        }
    }
    {
        auto l  = getComponents();
        auto r  = right.getComponents();
        if(l.size()  == r.size()) {
            for(const auto &it : l) {
                const auto &c   = r.find(it.first);
                if(c != r.end()) {
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

bool AObject::operator!=(const AObject &right) const {
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
