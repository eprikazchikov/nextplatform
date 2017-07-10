#include "aobject.h"

#include "aobjectsystem.h"
#include "auri.h"

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
        m_pCurrentSender(nullptr),
        m_UUID(0) {
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
        AObject *c  = it;
        if(c) {
            c->m_pParent    = 0;
            delete c;
        }
    }
    m_mChildren.clear();

    if(m_pParent) {
        m_pParent->removeChild(this);
    }
}

AObject *AObject::construct() {
    PROFILE_FUNCTION()
    return new AObject();
}

const AMetaObject *AObject::metaClass() {
    PROFILE_FUNCTION()
    static const AMetaObject staticMetaData("AObject", nullptr, &construct, nullptr, nullptr);
    return &staticMetaData;
}

const AMetaObject *AObject::metaObject() const {
    PROFILE_FUNCTION()
    return AObject::metaClass();
}

AObject *AObject::clone() {
    PROFILE_FUNCTION()
    const AMetaObject *meta = metaObject();
    AObject *result = meta->createInstance();
    for(auto it : getDynamicProperties()) {
        result->setProperty(it.first.c_str(), it.second);
    }
    uint32_t count  = meta->propertyCount();
    for(int i = 0; i < count; i++) {
        AMetaProperty lp    = result->metaObject()->property(i);
        AMetaProperty rp    = meta->property(i);
        lp.write(result, rp.read(this));
    }
    for(auto it : getChildren()) {
        AObject *clone  = it->clone();
        clone->setName(it->name());
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

uint32_t AObject::uuid() const {
    PROFILE_FUNCTION()
    return m_UUID;
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

const AObject::ObjectList &AObject::getChildren() const {
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
            AObject *o  = it->find(path.substr(index + 1));
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
        m_pParent->removeChild(this);
    }
    if(parent) {
        parent->addChild(this);
    }
    m_pParent   = parent;
}

void AObject::setName(const string &value) {
    PROFILE_FUNCTION()
    if(!value.empty()) {
        m_sName = value;
        // \todo Notify receivers
    }
}

void AObject::addChild(AObject *value) {
    PROFILE_FUNCTION()
    if(value) {
        m_mChildren.push_back(value);
    }
}

void AObject::removeChild(AObject *value) {
    PROFILE_FUNCTION()
    auto it = m_mChildren.begin();
    while(it != m_mChildren.end()) {
        if(*it == value) {
            m_mChildren.erase(it);
            return;
        }
        it++;
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

void AObject::setEnable(bool state) {
    PROFILE_FUNCTION()
    m_bEnable   = state;
}

bool AObject::event(AEvent *e) {
    PROFILE_FUNCTION()
    return false;
}

void AObject::loadUserData(const AVariant &data) {

}

AVariant AObject::saveUserData() const {
    return AVariant();
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
