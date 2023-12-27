#ifndef SYSTEM_H
#define SYSTEM_H

#include "blocks/Integrator/integrator.h"

class System
{
public:
    System(float value);
    float getOutput() const;
    float update(float value, float dt);

private:
    Integrator *m_integrator;
};

#endif // SYSTEM_H
