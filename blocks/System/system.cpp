#include "system.h"


System::System(float value)
{
    m_integrator = new Integrator(value);
}

float System::getOutput() const
{
    return m_integrator->state();
}

float System::update(float value, float dt)
{
    float tmp = m_integrator->state();

    m_integrator->update(value - tmp, dt);

    return tmp;
}
