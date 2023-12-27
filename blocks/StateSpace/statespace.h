#ifndef STATESPACE_H
#define STATESPACE_H

#include "blocks/integrator/integrator.h"
#include <vector>
#include <cstdint>

class StateSpace
{
public:
    StateSpace(std::vector<float> &m_initial_conditions,
               std::vector<std::vector<float>> &matrix_A,
               std::vector<float> &matrix_B,
               std::vector<float> &matrix_C,
               std::vector<float> &matrix_D);

    ~StateSpace();

    float getOutput();
    float update(float input, float dt);

private:
    Integrator *m_integrator_X1;
    Integrator *m_integrator_X2;
    Integrator *m_integrator_X3;

    float m_previous_input = 0.0;

    std::vector<std::vector<float>> m_matrix_A;
    std::vector<float> m_matrix_B;
    std::vector<float> m_matrix_C;
    std::vector<float> m_matrix_D;

    std::vector<float> m_initial_conditions;
};

#endif // STATESPACE_H
