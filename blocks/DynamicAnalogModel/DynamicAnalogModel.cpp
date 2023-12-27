#include "DynamicAnalogModel.h"


DynamicAnalogModel::DynamicAnalogModel(std::vector<float> &initial_conditions,
                     std::vector<std::vector<float>> &matrix_A,
                     std::vector<float> &matrix_B,
                     std::vector<float> &matrix_C,
                     std::vector<float> &matrix_D)

{
    m_integrator_X1 = new Integrator(initial_conditions[0]);
    m_integrator_X2 = new Integrator(initial_conditions[1]);

    m_matrix_A.resize(2);

    for(uint64_t i = 0; i != matrix_A.size(); ++i){
        m_matrix_A[i].resize(2);
    }

    for(uint64_t i = 0; i != matrix_A.size(); ++i){
        for(uint64_t j = 0; j != matrix_A[i].size(); ++j){
            m_matrix_A[i][j] = matrix_A[i][j];
        }
    }

    for(uint64_t i = 0; i != matrix_B.size(); ++i){
        m_matrix_B.push_back(matrix_B[i]);
    }

    for(uint64_t i = 0; i != matrix_C.size(); ++i){
        m_matrix_C.push_back(matrix_C[i]);
    }

    for(uint64_t i = 0; i != matrix_D.size(); ++i){
        m_matrix_D.push_back(matrix_D[i]);
    }

}

DynamicAnalogModel::~DynamicAnalogModel()
{
    delete m_integrator_X1;
    delete m_integrator_X2;
}

float DynamicAnalogModel::getOutput()
{
    return m_matrix_C[0] * m_integrator_X1->state() +
           m_matrix_C[1] * m_integrator_X2->state() +
           m_matrix_D[0] * m_previous_input;
}

float DynamicAnalogModel::update(float input, float dt)
{
    float tmp_x1 = m_integrator_X1->state();
    float tmp_x2 = m_integrator_X2->state();
    float output = getOutput();

    m_integrator_X1->update((m_matrix_A[0][0] * tmp_x1) + (m_matrix_A[0][1] * tmp_x2)  + (m_matrix_B[0] * input), dt);
    m_integrator_X2->update((m_matrix_A[1][0] * tmp_x1) + (m_matrix_A[1][1] * tmp_x2)  + (m_matrix_B[1] * input), dt);

    m_previous_input = input;

    return output;
}
