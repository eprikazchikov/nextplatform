#include "aobject.h"

#include "aobjectsystem.h"
#include "auri.h"

#define TYPE        "Type"
#define NAME        "Name"
#define LINKS       "Links"
#define ENABLE      "Enable"
#define PROPERTIES  "Properties"
#define COMPONENTS  "Components"

inline bool operator==(const AObject::Link &left, const AObject::Link &right) {
    bool result = true;
    result &= (left.sender      == right.sender);
    result &= (left.receiver    == right.receiver);
    result &= (left.signal      == right.signal);
    result &= (left.method      == right.method);
    return result;
}

AObject::AObject() :
        m_bEnable(true),
        m_pParent(nullptr),
        m_pCurrentSender(nullptr) {
    PROFILE_FUNCTION()

    onCreated();
}

AObject::~AObject() {
    PROFILE_FUNCTION()
    onDestroyed();

    {
        unique_lock<mutex> locker(m_Mutex);
        while(!m_EventQueue.empty()) {
            delete m_EventQueue.front();
            m_EventQueue.pop();
        }
    }

    while(!m_lSenders.empty()) {
        disconnect(m_lSenders.front().sender, 0, this, 0);
    }
    disconnect(this, 0, 0, 0);

    for(const auto &it : m_mChildren) {
        AObject *c  = it.second;
        if(c) {
            c->m_pParent    = 0;
            delete c;
        }
    }
    m_mChildren.clear();

    if(m_pParent) {
        m_pParent->removeChild(m_sName);
    }
}

AObject *AObject::createObject() {
    PROFILE_FUNCTION()
    return new AObject();
}

const AMetaObject *AObject::metaClass() {
    PROFILE_FUNCTION()
    static const AMetaObject staticMetaData("AObject", nullptr, &createObject, nullptr, nullptr);
    return &staticMetaData;
}

const AMetaObject *AObject::metaObject() const {
    PROFILE_FUNCTION()
    return AObject::metaClass();
}

AObject *AObject::clone() {
    PROFILE_FUNCTION()
    AObject *result = metaObject()->createInstance();
    for(auto it : getDynamicProperties()) {
        result->setProperty(it.first.c_str(), it.second);
    }
    for(int i = 0; i < metaObject()->propertyCount(); i++) {
        AMetaProperty lp    = result->metaObject()->property(i);
        AMetaProperty rp    = metaObject()->property(i);
        lp.write(result, rp.read(this));
    }
    for(auto it : getChildren()) {
        AObject *clone  = it.second->clone();
        clone->setName(it.first);
        clone->setParent(result);
    }
    for(auto it : getSenders()) {
        AMetaMethod signal  = it.sender->metaObject()->method(it.signal);
        AMetaMethod method  = result->metaObject()->method(it.method);
        connect(it.sender, (to_string(1) + signal.signature()).c_str(),
                result, (to_string((method.type() == AMetaMethod::Signal) ? 1 : 2) + method.signature()).c_str());
    }
    for(auto it : getReceivers()) {
        AMetaMethod signal  = result->metaObject()->method(it.signal);
        AMetaMethod method  = it.receiver->metaObject()->method(it.method);
        connect(result, (to_string(1) + signal.signature()).c_str(),
                it.receiver, (to_string((method.type() == AMetaMethod::Signal) ? 1 : 2) + method.signature()).c_str());
    }
    return result;
}

AObject *AObject::parent() const {
    PROFILE_FUNCTION()
    return m_pParent;
}

string AObject::name() const {
    PROFILE_FUNCTION()
    return m_sName;
}

string AObject::typeName() const {
    PROFILE_FUNCTION()
    return metaObject()->name();
}

void AObject::connect(AObject *sender, const char *signal, AObject *receiver, const char *method) {
    PROFILE_FUNCTION()
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
                {
                    unique_lock<mutex> locker(sender->m_Mutex);
                    sender->m_lRecievers.push_back(link);
                }
                {
                    unique_lock<mutex> locker(receiver->m_Mutex);
                    receiver->m_lSenders.push_back(link);
                }
            }
        }
    }
}

void AObject::disconnect(AObject *sender, const char *signal, AObject *receiver, const char *method) {
    PROFILE_FUNCTION()
    if(sender && !sender->m_lRecievers.empty()) {
        for(auto snd = sender->m_lRecievers.begin(); snd != sender->m_lRecievers.end(); snd) {
            Link *data = &(*snd);

            if(data->sender == sender) {
                if(signal == nullptr || data->signal == sender->metaObject()->indexOfMethod(&signal[1])) {
                    if(receiver == nullptr || data->receiver == receiver) {
                        if(method == nullptr || (receiver && data->method == receiver->metaObject()->indexOfMethod(&method[1]))) {

                            for(auto rcv = data->receiver->m_lSenders.begin(); rcv != data->receiver->m_lSenders.end(); rcv) {
                                if(*rcv == *data) {
                                    unique_lock<mutex> locker(data->receiver->m_Mutex);
                                    rcv = data->receiver->m_lSenders.erase(rcv);
                                } else {
                                    rcv++;
                                }
                            }
                            unique_lock<mutex> locker(sender->m_Mutex);
                            snd = sender->m_lRecievers.erase(snd);

                            continue;
                        }
                    }
                }
            }
            snd++;
        }
    }
}

void AObject::deleteLater() {
    PROFILE_FUNCTION()
    postEvent(new AEvent(AEvent::Delete));
}

const AObject::ObjectMap &AObject::getChildren() const {
    PROFILE_FUNCTION()
    return m_mChildren;
}

const AObject::LinkList &AObject::getReceivers() const {
    PROFILE_FUNCTION()
    return m_lRecievers;
}

const AObject::LinkList &AObject::getSenders() const {
    PROFILE_FUNCTION()
    return m_lSenders;
}

const AObject::PropertyMap &AObject::getDynamicProperties() const {
    PROFILE_FUNCTION()
    return m_mDynamicProperties;
}

AObject *AObject::find(const string &path) {
    PROFILE_FUNCTION()
    if(m_pParent && path[0] == '/') {
        return m_pParent->find(path);
    }

    unsigned int start  = 0;
    if(path[0] == '/') {
        start   = 1;
    }
    int index  = path.find('/', 1);
    if(index > -1) {
        for(const auto &it : m_mChildren) {
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
    PROFILE_FUNCTION()
    if(m_pParent) {
        m_pParent->removeChild(m_sName);
    }
    if(parent) {
        parent->addChild(this, m_sName);
    }
    m_pParent   = parent;
}

void AObject::setName(const string &value) {
    PROFILE_FUNCTION()
    if(!value.empty()) {
        if(m_sName != value && m_pParent) {
            m_pParent->removeChild(m_sName);
            m_pParent->addChild(this, value);
        }
        m_sName = value;
    }
}

void AObject::addChild(AObject *value, const string &name) {
    PROFILE_FUNCTION()
    if(value && !name.empty()) {
        auto it = m_mChildren.find(name);
        if(it != m_mChildren.end()) {
            delete (*it).second;
        }
        m_mChildren[name]    = value;
    }
}

void AObject::removeChild(const string &name) {
    PROFILE_FUNCTION()
    auto it = m_mChildren.find(name);
    if(it != m_mChildren.end()) {
        m_mChildren.erase(it);
    }
}

bool AObject::isEnable() const {
    PROFILE_FUNCTION()
    return m_bEnable;
}

void AObject::emitSignal(const char *signal, const AVariant &args) {
    PROFILE_FUNCTION()
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
    PROFILE_FUNCTION()
    unique_lock<mutex> locker(m_Mutex);
    m_EventQueue.push(e);

    return true;
}

void AObject::processEvents() {
    PROFILE_FUNCTION()
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
    PROFILE_FUNCTION()
    if(m_pParent) {
        return m_pParent->reference() + "/" + m_sName;
    }
    return string("thor://") + AObjectSystem::instance()->name() + "/" + m_sName;
}

void AObject::setEnable(bool state) {
    PROFILE_FUNCTION()
    m_bEnable   = state;
}

bool AObject::event(AEvent *e) {
    PROFILE_FUNCTION()
    return false;
}

AVariant AObject::property(const char *name) const {
    PROFILE_FUNCTION()
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
    PROFILE_FUNCTION()
    const AMetaObject *meta = metaObject();
    int index   = meta->indexOfProperty(name);
    if(index > -1) {
        meta->property(index).write(this, value);
    } else {
        m_mDynamicProperties[name]  = value;
    }
}

void AObject::onCreated() {
    PROFILE_FUNCTION()

}

void AObject::onDestroyed() {
    PROFILE_FUNCTION()

}

AObject *AObject::sender() const {
    PROFILE_FUNCTION()
    return m_pCurrentSender;
}

AVariant AObject::toVariant() {
    PROFILE_FUNCTION()
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
        for(const auto &it : m_mChildren) {
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
    PROFILE_FUNCTION()
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
            if(m_mChildren.find(it.first) == m_mChildren.end()) {
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
    PROFILE_FUNCTION()
    AVariant::AVariantMap map   = variant.toMap();

    AObject *result = AObjectSystem::objectCreate(map[TYPE].toString(), "", parent);
    if(result) {
        result->fromVariant(variant);
    }
    return result;
}

bool AObject::isLinkExist(const Link &link) const {
    PROFILE_FUNCTION()
    for(const auto &it : m_lRecievers) {
        if(it == link) {
            return true;
        }
    }
    return false;
}

AObject &AObject::operator=(AObject &right) {
    PROFILE_FUNCTION()
    return AObject(right);
}

AObject::AObject(const AObject &) {
    PROFILE_FUNCTION()
}
