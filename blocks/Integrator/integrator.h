#ifndef INTEGRATOR_H
#define INTEGRATOR_H


class Integrator
{
public:
    Integrator(float value);

    float state() const;
    float update(float input, float dt);

private:
    float m_state = 0.0;
    float m_previous_input = 0.0;
};

#endif // INTEGRATOR_H
