#include "integrator.h"

Integrator::Integrator(float value)
{
    m_state = value;
}

float Integrator::state() const
{
    return m_state;
}

float Integrator::update(float input, float dt)
{
    m_state +=  (m_previous_input + input) * dt / 2;
    m_previous_input = input;
    return m_state;
}
