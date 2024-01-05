#include "statespace.h"
StateSpace::StateSpace(std::vector<float> &m_initial_conditions,
                       std::vector<std::vector<float>> &matrix_A,
                       std::vector<float> &matrix_B,
                       std::vector<float> &matrix_C,
                       std::vector<float> &matrix_D)

{
    m_integrator_X1 = new Integrator(m_initial_conditions[0]);
    m_integrator_X2 = new Integrator(m_initial_conditions[1]);
    m_integrator_X3 = new Integrator(m_initial_conditions[2]);

    m_matrix_A.resize(3);

    for(uint64_t i = 0; i != matrix_A.size(); ++i){  m_matrix_A[i].resize(3); }
    for(uint64_t i = 0; i != matrix_A.size(); ++i){
        for(uint64_t j = 0; j != matrix_A[i].size(); ++j){ m_matrix_A[i][j] = matrix_A[i][j]; }}
    for(uint64_t i = 0; i != matrix_B.size(); ++i){ m_matrix_B.push_back(matrix_B[i]); }
    for(uint64_t i = 0; i != matrix_C.size(); ++i){ m_matrix_C.push_back(matrix_C[i]);}
    for(uint64_t i = 0; i != matrix_D.size(); ++i){ m_matrix_D.push_back(matrix_D[i]); }

}

StateSpace::~StateSpace()
{
    delete m_integrator_X1;
    delete m_integrator_X2;
    delete m_integrator_X3;
}

float StateSpace::getOutput()
{
    float itgState1 = m_integrator_X2->state();
    float itgState2 = m_integrator_X2->state();
    float itgState3 = m_integrator_X3->state();

    float out1 = itgState1 * m_matrix_C[0];
    float out2 = itgState2 * m_matrix_C[1];
    float out3 = itgState3 * m_matrix_C[2];
    float in = m_matrix_D[0] * m_previous_input;
    return out1 + out2 + out3 + in;
}

float StateSpace::update(float input, float dt)
{
    float tmp_x1 = m_integrator_X1->state();
    float tmp_x2 = m_integrator_X2->state();
    float tmp_x3 = m_integrator_X3->state();
    float output = getOutput();

    m_integrator_X1->update((m_matrix_A[0][0] * tmp_x1) + (m_matrix_A[0][1] * tmp_x2) + (m_matrix_A[0][2] * tmp_x3) + (m_matrix_B[0] * input), dt);
    m_integrator_X2->update((m_matrix_A[1][0] * tmp_x1) + (m_matrix_A[1][1] * tmp_x2) + (m_matrix_A[1][2] * tmp_x3) + (m_matrix_B[1] * input), dt);
    m_integrator_X3->update((m_matrix_A[2][0] * tmp_x1) + (m_matrix_A[2][1] * tmp_x2) + (m_matrix_A[1][2] * tmp_x3) + (m_matrix_B[2] * input), dt);

    m_previous_input = input;

    return output;
}
