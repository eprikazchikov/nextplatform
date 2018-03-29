#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>

#include <common.h>

class NEXT_LIBRARY_EXPORT Event {
public:
    enum Type {
        INVALID                 = 0,
        METHODCALL,
        TIMER,
        DELETE,
        USERTYPE                = 100
    };

public:
    Event                       (uint32_t type);

    uint32_t                    type                        () const;

protected:
    uint32_t                    m_Type;
};

#endif // EVENT_H
