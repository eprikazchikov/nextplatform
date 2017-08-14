#ifndef AOBJECTSYSTEM_P_H
#define AOBJECTSYSTEM_P_H

#include "aobjectsystem.h"

class AObjectSystemPrivate {
public:
    AObjectSystemPrivate() :
        m_Exit(false) {
    }

    /// Container for registered callbacks.
    AObjectSystem::FactoryMap   m_Factories;
    AObjectSystem::GroupMap     m_Groups;

    bool                        m_Exit;

    uint32_t                    m_NextID;
};

#endif // AOBJECTSYSTEM_P_H
