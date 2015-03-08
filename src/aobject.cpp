#include "aobject.h"

#include "aobjectsystem.h"
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
#define PROPERTIES  "Properties"
#define COMPONENTS  "Components"

AObject::AObject(AObject *parent) {
    m_bEnable   = true;
    m_bDelete   = false;

    m_id        = AObjectSystem::instance()->nextId();

    m_sName     = typeName() + "_" +  to_string(m_id);

    setParent(parent);
}

AObject::~AObject() {
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

void AObject::addLink(const link_data &link) {
    /// \todo: Place for link_mutex
    for(const auto &it : m_lLinks) {
        if(it.signal == link.signal && it.slot == link.slot &&
           it.sender == link.sender && it.receiver == link.receiver) {
            return;
        }
    }
    m_lLinks.push_back(link);
}

void AObject::removeLink(const link_data &link) {
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

AVariant AObject::getLinks(const string &name) const {
    AVariant result(AVariant::LIST);
    for(const auto &link : m_lLinks) {
        if(link.sender == this && link.signal == name) {
            result.appendProperty(link.receiver->reference() + "?" + link.slot);
        } else if(link.receiver == this && link.slot == name) {
            result.appendProperty(link.sender->reference() + "#" + link.signal);
        }
    }
    return result;
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
        parent->setComponent(m_sName, this);
    }
    m_pParent   = parent;
}

void AObject::setName(const string &value) {
    if(!value.empty()) {
        if(m_sName != value && m_pParent) {
            auto it =  m_pParent->m_mComponents.find(m_sName);
            if(it !=  m_pParent->m_mComponents.end()) {
                 m_pParent->m_mComponents.erase(it);
            }

            m_pParent->setComponent(value, this);
        }
        m_sName = value;
    }
}

void AObject::setComponent(const string &name, AObject *value) {
    if(value && !name.empty()) {
        auto it = m_mComponents.find(m_sName);
        if(it != m_mComponents.end()) {
            delete (*it).second;
            m_mComponents.erase(it);
        }

        m_mComponents[name] = value;

        value->m_pParent    = this;
        value->m_sName      = name;
    }
}

bool AObject::isEnable() const {
    return m_bEnable;
}

AObject *AObject::callbackClassFactory(AObject *parent) {
    return new AObject(parent);
}

void AObject::registerClassFactory() {
    AObject object;
    AObjectSystem::instance()->factoryAdd(object.typeName(), &AObject::callbackClassFactory);
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
            callback call   = m_mSlots[name];
            if(call) {
                (*call)(this, args);
            } else {
                onEvent(name, args);
            }
        }
    }
}

void AObject::onEvent(const string &name, const variant_vector &args) {

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
    if(m_pParent) {
        return m_pParent->reference() + "/" + m_sName;
    }
    return string("thor://" + AObjectSystem::instance()->systemName() + "/" + m_sName);
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

void AObject::setPropertySettings(const string &name, const int flags, const int type, const int order) {
    auto &it    = m_mProperties[name];
    it.flags    = (AccessTypes)flags;
    it.type     = type;
    it.order    = order;
}

AVariant AObject::toVariant(const AObject &object) {
    AVariant result(AVariant::MAP);

    result.appendProperty(object.typeName(),    TYPE);
    result.appendProperty(object.m_sName,       NAME);
    result.appendProperty(object.m_bEnable,     ENABLE);

    {
        AVariant s(AVariant::LIST);
        for(const auto &it : object.m_mProperties) {
            AVariant p(AVariant::MAP);
            p.appendProperty(it.first,          NAME);
            p.appendProperty(it.second.flags,   FLAGS);
            p.appendProperty(it.second.data,    DATA);
            p.appendProperty(it.second.type,    TYPE);
            p.appendProperty(it.second.order,   ORDER);
            p.appendProperty(object.getLinks(it.first), LINKS);

            s.appendProperty(p);
        }
        result.appendProperty(s, PROPERTIES);
    }

    {
        AVariant s(AVariant::LIST);
        for(const auto &it : object.m_mSignals) {
            AVariant m(AVariant::MAP);
            m.appendProperty(it, NAME);
            m.appendProperty(object.getLinks(it), LINKS);
            s.appendProperty(m);
        }
        result.appendProperty(s, SIGNALS);
    }

    {
        AVariant s(AVariant::LIST);
        for(const auto &it : object.m_mSlots) {
            AVariant m(AVariant::MAP);
            m.appendProperty(it.first, NAME);
            m.appendProperty(object.getLinks(it.first), LINKS);
            s.appendProperty(m);
        }
        result.appendProperty(s, SLOTS);
    }

    {
        AVariant components(AVariant::MAP);
        for(const auto &it : object.m_mComponents) {
            if(it.second) {
                components.appendProperty(toVariant(*it.second), it.first);
            } else {
                components.appendProperty(NULL, it.first);
            }
        }
        result.appendProperty(components, COMPONENTS);
    }

    return result;
}

AObject *AObject::toObject(const AVariant &variant, AObject *parent) {
    AVariant::AVariantMap map   = variant.toMap();
    string type     = map[TYPE].toString();

    AObject *result = AObjectSystem::instance()->createObject(type, parent);
    if(result) {
        result->setName     (map[NAME].toString());
        result->setEnable   (map[ENABLE].toBool());

        {
            for(const auto &it : map[COMPONENTS].toMap()) {
                if(result->m_mComponents.find(it.first) == result->m_mComponents.end()) {
                    toObject(it.second, result);
                }
            }
        }

        {
            for(const auto &it : map[SLOTS].toList()) {
                auto m  = it.toMap();
                if(result->m_mSlots.find(m[NAME].toString()) != result->m_mSlots.end()) {
                    result->m_mSlots[m[NAME].toString()]    = NULL;
                }

                for(const auto &link : m[LINKS].toList()) {
                    AUri uri(link.toString());
                    AObject *o  = result->findObject(uri.path());
                    if(o) {
                        addEventListner(o, uri.fragment(), result, m[NAME].toString());
                    }
                }
            }
        }

        {
            for(const auto &it : map[SIGNALS].toList()) {
                auto m      = it.toMap();
                bool create = true;
                for(const auto &it : result->m_mSignals) {
                    if(it == m[NAME].toString()) {
                        create  = false;
                        break;
                    }
                }

                if(create) {
                    result->m_mSignals.push_back(m[NAME].toString());
                }
                for(const auto &link : m[LINKS].toList()) {
                    AUri uri(link.toString());
                    AObject *o  = result->findObject(uri.path());
                    if(o) {
                        addEventListner(result, m[NAME].toString(), o, uri.query());
                    }
                }
            }
        }
        {
            for(const auto &it : map[PROPERTIES].toList()) {
                AVariant::AVariantMap p = it.toMap();
                result->setProperty(p[NAME].toString(), p[DATA]);
                result->setPropertySettings(p[NAME].toString(), p[FLAGS].toInt(), p[TYPE].toInt(), p[ORDER].toInt());

                for(const auto &link : p[LINKS].toList()) {
                    AUri uri(link.toString());
                    AObject *o  = result->findObject(uri.path());
                    if(o) {
                        if(uri.fragment().empty()) {
                            addEventListner(result, p[NAME].toString(), o, uri.query());
                        } else {
                            addEventListner(o, uri.fragment(), result, p[NAME].toString());
                        }
                    }
                }
            }
        }
    }
    return result;
}

bool AObject::operator==(const AObject &right) const {
    bool result = true;

    result &= m_bEnable     == right.m_bEnable;
    result &= m_mSignals    == right.m_mSignals;
    result &= m_mProperties == right.m_mProperties;
    result &= m_lLinks      == right.m_lLinks;

    if(!result)
        return result;

    {
        if(m_mSlots.size()  == right.m_mSlots.size()) {
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
                const auto &r   = right.m_mComponents.find(it.first);
                if(r != right.m_mComponents.end()) {
                    if(*(it.second) != *(r->second)) {
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

inline bool operator==(const AObject::property_data &left, const AObject::property_data &right) {
    bool result = true;
    result &= left.flags    == right.flags;
    result &= left.data     == right.data;
    result &= left.type     == right.type;
    return result;
}

inline bool operator==(const AObject::link_data &left, const AObject::link_data &right) {
    bool result = true;
    result &= left.signal   == right.signal;
    result &= left.slot     == right.slot;
    return result;
}
