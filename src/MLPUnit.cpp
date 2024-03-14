#include "MLPUnit.hpp"

#include <cmath>
#include <cassert>

MlpUnit::MlpUnit() {
    remain_cycle_ = 0;
}

void MlpUnit::Cycle(){
    if (remain_cycle_ == 0) {
        // MLP computation is started. 711-1=710
        remain_cycle_ = 710;
        
        // Convert input feature to matrix form
        vector<vector<float>> in_feature_matrix;
        for (auto &in_feature : in_features){
            in_feature_matrix.push_back(in_feature.feature_vector);
        }
        assert(in_feature_matrix.size() == 32);
        assert(in_feature_matrix[0].size() == 32);

        // Density network
        vector<vector<float>> out_dens = matrix_multiply(matrix_multiply(in_feature_matrix, w1_density_), w2_density_); 

        // Spherical harmonic
        vector<vector<float>> out_sphe;

        // Color network 
        vector<vector<float>> out_cols = matrix_multiply(matrix_multiply(matrix_multiply(out_sphe, w1_color_), w2_color_), w3_color_); 
        
        //Compute final color

        
        // Now clear inputs
        this->ClearInputs();

    } else {
        // if MLPUnit doesn't spend 711 cycles, just decrease remain_cycle_.
        remain_cycle_ -= 1;
    }

}

void MlpUnit::ClearInputs() {
    this->in_features.clear();
}

vector<float> ComputeColor() {

}

vector<vector<float>> matrix_multiply(const vector<vector<float>>& A, const vector<vector<float>>& B) {
    int m = A.size();
    int n = B[0].size();
    int p = B.size();    
    vector<vector<float>> result(m, vector<float>(n, 0.0));

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < p; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}