#include "aobject.h"

#include "aobjectsystem.h"
#include "auri.h"
#include "atools.h"

#define TYPE        "Type"
#define NAME        "Name"
#define LINKS       "Links"
#define ENABLE      "Enable"
#define PROPERTIES  "Properties"
#define COMPONENTS  "Components"

AObject::AObject(AObject *parent) :
        m_bEnable(true),
        m_pParent(nullptr),
        m_pCurrentSender(nullptr) {

    setParent(parent);

    onCreated();
}

AObject::~AObject() {
    onDestroyed();

    unique_lock<mutex> locker(m_Mutex);
    while(!m_EventQueue.empty()) {
        delete m_EventQueue.front();
        m_EventQueue.pop();
    }

    while(!m_lSenders.empty()) {
        disconnect(m_lSenders.front().sender, 0, this, 0);
    }
    disconnect(this, 0, 0, 0);

    for(const auto &it : m_mComponents) {
        AObject *c  = it.second;
        if(c) {
            c->m_pParent    = 0;
            delete c;
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

AObject *AObject::createObject(AObject *parent) {
    return new AObject(parent);
}

const AMetaObject *AObject::metaClass() {
    static const AMetaObject staticMetaData("AObject", nullptr, &createObject, nullptr, nullptr);
    return &staticMetaData;
}

const AMetaObject *AObject::metaObject() const {
    return AObject::metaClass();
}

AObject *AObject::parent() const {
    return m_pParent;
}

string AObject::name() const {
    return m_sName;
}

string AObject::typeName() const {
    return metaObject()->name();
}

void AObject::connect(AObject *sender, const char *signal, AObject *receiver, const char *method) {
    if(sender && receiver) {
        int32_t snd = sender->metaObject()->indexOfSignal(&signal[1]);

        int32_t rcv;
        AMetaMethod::MethodType right   = AMetaMethod::MethodType(method[0] - 0x30);
        if(right == AMetaMethod::Slot) {
            rcv = receiver->metaObject()->indexOfSlot(&method[1]);
        } else {
            rcv = receiver->metaObject()->indexOfSignal(&method[1]);
        }

        if(snd > -1 && rcv > -1) {
            Link link;

            link.sender     = sender;
            link.signal     = snd;
            link.receiver   = receiver;
            link.method     = rcv;
/*
            link.reference  = receiver->reference() + "?" + char(method.type() + 0x30) + method.signature();
            sender->reference() + "#" + char(method.type() + 0x30) + method.signature();
*/
            if(!sender->isLinkExist(link)) {
                //sender->m_Mutex.lock();
                    sender->m_lRecievers.push_back(link);
                //sender->m_Mutex.unlock();

                //receiver->m_Mutex.lock();
                    receiver->m_lSenders.push_back(link);
                //receiver->m_Mutex.unlock();
            }
        } else {
            // Can't connect Signal to Slot
        }
    }
    /// \todo: Should be thread safe
}

void AObject::disconnect(AObject *sender, const char *signal, AObject *receiver, const char *method) {
    if(sender) {
        for(auto snd = sender->m_lRecievers.begin(); snd != sender->m_lRecievers.end(); snd) {
            Link *data = &(*snd);

            if(data->sender == sender) {
                if(signal == nullptr || data->signal == sender->metaObject()->indexOfMethod(&signal[1])) {
                    if(receiver == nullptr || data->receiver == receiver) {
                        if(method == nullptr || (receiver && data->method == receiver->metaObject()->indexOfMethod(&method[1]))) {

                            for(auto rcv = data->receiver->m_lSenders.begin(); rcv != data->receiver->m_lSenders.end(); rcv) {
                                if(*rcv == *data) {
                                    //unique_lock<mutex> locker(data->receiver->m_Mutex);
                                    rcv = data->receiver->m_lSenders.erase(rcv);
                                } else {
                                    rcv++;
                                }
                            }
                            //unique_lock<mutex> locker(sender->m_Mutex);
                            snd = sender->m_lRecievers.erase(snd);

                            continue;
                        }
                    }
                }
            }

            snd++;
        }
    }
    /// \todo: Should be thread safe
}

void AObject::deleteLater() {
    postEvent(new AEvent(AEvent::Delete));
}

AObject::ObjectMap &AObject::getComponents() {
    return m_mComponents;
}

AObject::LinkList &AObject::getReceivers() {
    return m_lRecievers;
}

AObject::LinkList &AObject::getSenders() {
    return m_lSenders;
}

AObject *AObject::find(const string &path) {
    if(m_pParent && path[0] == '/') {
        return m_pParent->find(path);
    }

    unsigned int start  = 0;
    if(path[0] == '/') {
        start   = 1;
    }
    int index  = path.find('/', 1);
    if(index > -1) {
        for(const auto &it : m_mComponents) {
            AObject *o  = it.second->find(path.substr(index + 1));
            if(o) {
                return o;
            }
        }
    } else if(path.substr(start, index) == m_sName) {
        return this;
    }

    return nullptr;
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
            type    = value->metaObject()->name();
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

void AObject::emitSignal(const char *signal, const AVariant &args) {
    int32_t index   = metaObject()->indexOfSignal(&signal[1]);
    for(auto &it : m_lRecievers) {
        Link *link  = &(it);
        if(link->signal == index) {
            const AMetaMethod &method   = link->receiver->metaObject()->method(link->method);
            if(method.type() == AMetaMethod::Signal) {
                link->receiver->emitSignal(string(char(method.type() + 0x30) + method.signature()).c_str(), args);
            } else {
                AMethodCallEvent *event = new AMethodCallEvent(link->method, link->sender, args);
                link->receiver->postEvent(event);
            }
        }
    }
}

bool AObject::postEvent(AEvent *e) {
    unique_lock<mutex> locker(m_Mutex);
    m_EventQueue.push(e);

    return true;
}

void AObject::processEvents() {
    while(!m_EventQueue.empty()) {
        unique_lock<mutex> locker(m_Mutex);
        AEvent *e   = m_EventQueue.front();
        switch (e->type()) {
            case AEvent::MethodCall: {
                AMethodCallEvent *call  = reinterpret_cast<AMethodCallEvent *>(e);
                m_pCurrentSender    = call->sender();
                AVariant result;
                metaObject()->method(call->method()).invoke(this, result, 1, call->args());
                m_pCurrentSender    = nullptr;
            } break;
            case AEvent::Delete: {
                locker.unlock();
                delete this;
                return;
            } break;
            default: {
                event(e);
            } break;
        }
        delete e;
        m_EventQueue.pop();
    }
}

string AObject::reference() const {
    if(m_pParent) {
        return m_pParent->reference() + "/" + m_sName;
    }
    return string("thor://") + AObjectSystem::instance()->name() + "/" + m_sName;
}

void AObject::setEnable(bool state) {
    m_bEnable   = state;
}

bool AObject::event(AEvent *e) {
    return false;
}

AVariant AObject::property(const char *name) const {
    const AMetaObject *meta = metaObject();
    int index   = meta->indexOfProperty(name);
    if(index > -1) {
        return meta->property(index).read(this);
    } else {
        auto it = m_mDynamicProperties.find(name);
        if(it != m_mDynamicProperties.end()) {
            return it->second;
        }
    }
    return AVariant();
}

void AObject::setProperty(const char *name, const AVariant &value) {
    const AMetaObject *meta = metaObject();
    int index   = meta->indexOfProperty(name);
    if(index > -1) {
        meta->property(index).write(this, value);
    } else {
        m_mDynamicProperties[name]  = value;
    }
}

void AObject::onCreated() {

}

void AObject::onDestroyed() {

}

AObject *AObject::sender() const {
    return m_pCurrentSender;
}

AVariant AObject::toVariant() {
    AVariant::AVariantMap object;
    object[TYPE]    = typeName();
    object[NAME]    = m_sName;
    object[ENABLE]  = m_bEnable;

    const AMetaObject *meta = metaObject();
    // Save properties
    {
        AVariant::AVariantList s;
        for(uint32_t i = 0; i < meta->propertyCount(); i++) {
            AMetaProperty p = meta->property(i);
            if(p.isValid()) {
                AVariant::AVariantMap property;
                property[NAME]  = p.name();
                property[DATA]  = p.read(this);
                s.push_back(property);
            }
        }
        for(const auto it : m_mDynamicProperties) {
            AVariant::AVariantMap property;
            property[NAME]  = it.first;
            property[DATA]  = it.second;

            s.push_back(property);
        }
        object[PROPERTIES]  = s;
    }
    // Save components
    {
        AVariant::AVariantMap components;
        for(const auto &it : m_mComponents) {
            if(it.second) {
                AVariant left   = it.second->toVariant();
                components[it.first]    = left;
            } else {
                components[it.first]    = static_cast<void *>(nullptr);
            }
        }
        object[COMPONENTS]  = components;
    }
    // Save links
    {
        AVariant::AVariantList s;
        for(const auto &it : m_lRecievers) {
            AVariant::AVariantList link;

            AMetaMethod method  = metaObject()->method(it.signal);
            link.push_back(AVariant(char(method.type() + 0x30) + method.signature()));

            AObject *reciever   = it.receiver;
            method      = reciever->metaObject()->method(it.method);
            link.push_back(reciever->reference() + "?" + char(method.type() + 0x30) + method.signature());

            s.push_back(link);
        }
        for(const auto &it : m_lSenders) {
            AVariant::AVariantList link;

            AMetaMethod method  = metaObject()->method(it.method);
            link.push_back(AVariant(char(method.type() + 0x30) + method.signature()));

            AObject *sender = it.sender;
            method      = sender->metaObject()->method(it.signal);
            link.push_back(sender->reference() + "#" + char(method.type() + 0x30) + method.signature());

            s.push_back(link);
        }
        object[LINKS]   = s;
    }

    return object;
}

void AObject::fromVariant(const AVariant &variant) {
    AVariant::AVariantMap map   = variant.toMap();

    setName(map[NAME].toString());
    setEnable(map[ENABLE].toBool());

    // Load properties
    {
        for(const auto &it : map[PROPERTIES].toList()) {
            AVariant::AVariantMap m = it.toMap();

            setProperty(m[NAME].toString().c_str(), m[DATA]);
        }
    }
    // Load components
    {
        for(const auto &it : map[COMPONENTS].toMap()) {
            if(m_mComponents.find(it.first) == m_mComponents.end()) {
                toObject(it.second, this);
            }
        }
    }
    // Load links
    for(const auto &link : map[LINKS].toList()) {
        AVariant::AVariantList list = link.toList();

        AUri uri(list.back().toString());
        AObject *o  = find(uri.path());
        if(o) {
            if(uri.fragment().empty()) {
                connect(this, list.front().toString().c_str(), o, uri.query().c_str());
            } else {
                connect(o, uri.fragment().c_str(), this, list.front().toString().c_str());
            }
        }
    }
}

AObject *AObject::toObject(const AVariant &variant, AObject *parent) {
    AVariant::AVariantMap map   = variant.toMap();

    AObject *result = AObjectSystem::objectCreate(map[TYPE].toString(), parent);
    if(result) {
        result->fromVariant(variant);
    }
    return result;
}

bool AObject::isLinkExist(const Link &link) const {
    for(const auto &it : m_lRecievers) {
        if(it == link) {
            return true;
        }
    }
    return false;
}

AObject &AObject::operator=(AObject &right) {
    return AObject(right);
}

AObject::AObject(const AObject &) {
}

bool AObject::operator==(const AObject &right) {
    bool result = true;

    result &= (m_bEnable    == right.m_bEnable);
    result &= (typeName()   == right.typeName());

    result &= (m_mDynamicProperties == right.m_mDynamicProperties);

    if(!result) {
        return result;
    }

    {
        if(m_lRecievers.size() == right.m_lRecievers.size()) {
            for(const auto &li : m_lRecievers) {
                result  = false;
                for(const auto &ri : right.m_lRecievers) {
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

inline bool operator==(const AObject::Link &left, const AObject::Link &right) {
    bool result = true;
    result &= left.signal   == right.signal;
    result &= left.method   == right.method;
    return result;
}

inline bool operator!=(const AObject::Link &left, const AObject::Link &right) {
    return !(left == right);
}
