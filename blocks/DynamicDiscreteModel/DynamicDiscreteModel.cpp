#include "DynamicDiscreteModel.h"


DynamicDiscreteModel::DynamicDiscreteModel(std::vector<float> &initial_conditions,
                         std::vector<std::vector<float>> &matrix_A,
                         std::vector<float> &matrix_B,
                         std::vector<float> &matrix_C,
                         std::vector<float> &matrix_D)
{
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

    for(uint64_t i = 0; i != initial_conditions.size(); ++i){
        m_initial_conditions.push_back(initial_conditions[i]);
    }
}

float DynamicDiscreteModel::getOutput()
{
    return (m_initial_conditions[0] * m_matrix_C[0] +
            m_initial_conditions[1] * m_matrix_C[1] +
            m_previous_input * m_matrix_D[0]);
}

float DynamicDiscreteModel::update(float input)
{
    float output = getOutput();

    float tmp_x1 = m_initial_conditions[0];
    float tmp_x2 = m_initial_conditions[1];

    m_initial_conditions[0] = input * m_matrix_B[0] + tmp_x1 * m_matrix_A[0][0] + tmp_x2 * m_matrix_A[0][1];
    m_initial_conditions[1] = input * m_matrix_B[1] + tmp_x1 * m_matrix_A[1][0] + tmp_x2 * m_matrix_A[1][1];

    m_previous_input = input;

    return output;
}
