#include "StaticDiscreteModel.h"
#include <QDebug>
StaticDiscreteModel::StaticDiscreteModel(std::vector<float> &initial_conditions,
                   std::vector<std::vector<float>> &matrix_A,
                   std::vector<float> &matrix_B,
                   std::vector<float> &matrix_C,
                   std::vector<float> &matrix_D)
{
    m_matrix_A.resize(3);

    for(uint64_t i = 0; i != matrix_A.size(); ++i){
        m_matrix_A[i].resize(3);
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

float StaticDiscreteModel::getOutput()
{

    return m_initial_conditions[2] * m_matrix_C[2] + m_previous_input * m_matrix_D[0];
}

float StaticDiscreteModel::update(float input)
{
    float output = getOutput();

    float tmp_x1 = m_initial_conditions[0];
    float tmp_x2 = m_initial_conditions[1];
    float tmp_x3 = m_initial_conditions[2];

    float x1_out1 = tmp_x1 * m_matrix_A[0][0];
    float x1_out2 = tmp_x1 * m_matrix_A[1][0];
    float x1_out3 = tmp_x1 * m_matrix_A[2][0];

    float x2_out1 = tmp_x2 * m_matrix_A[0][1];
    float x2_out2 = tmp_x2 * m_matrix_A[1][1];
    float x2_out3 = tmp_x2 * m_matrix_A[2][1];

    float x3_out1 = tmp_x3 * m_matrix_A[0][2];
    float x3_out2 = tmp_x3 * m_matrix_A[1][2];
    float x3_out3 = tmp_x3 * m_matrix_A[2][2];

    float in1 = input * m_matrix_B[0];
    float in2 = input * m_matrix_B[1];
    float in3 = input * m_matrix_B[2];

    m_initial_conditions[0] = in1 + x1_out1 + x2_out1 + x3_out1;
    m_initial_conditions[1] = in2 + x1_out2 + x2_out2 + x3_out2;
    m_initial_conditions[2] = in3 + x1_out3 + x2_out3 + x3_out3;
    m_previous_input = input;

    return output;
}










