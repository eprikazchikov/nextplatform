#include "aevent.h"

AEvent::AEvent(Type type) {
    m_Type  = type;
}

AEvent::Type AEvent::type() const {
    return m_Type;
}
