#ifndef AEVENT_H
#define AEVENT_H

#include <stdint.h>

class AEvent {
public:
    enum Type {
        Invalid                 = 0,
        MethodCall,
        Delete,
        UserType
    };

public:
    AEvent                      (Type type);

    Type                        type                        () const;

protected:
    Type                        m_Type;
};

#endif // AEVENT_H
