#include "MLPUnit.hpp"

#include <cmath>
#include <cassert>

using ActivationFunction = float (*)(float);

MlpUnit::MlpUnit() {
    remain_cycle_ = 0;
}

void MlpUnit::Cycle(){
    if (remain_cycle_ == 0) {
        // MLP computation is started. 711-1=710
        remain_cycle_ = 710;
        
        // Convert input feature to matrix form
        vector<vector<float>> in_feature_matrix;
        for (auto &in_feature : in_features) {
            in_feature_matrix.push_back(in_feature.feature_vector);
        }
        assert(in_feature_matrix.size() == 32);
        assert(in_feature_matrix[0].size() == 32);

        // Density network
        vector<vector<float>> out_dens = matrix_multiply(matrix_multiply(in_feature_matrix, w1_density_, relu), w2_density_, relu); 

        for (auto &out_den : out_dens) {
            densities_.push_back(exp(out_den[0]));
            accumulate_dens_ += exp(out_den[0]);
        }

        // Spherical harmonic
        vector<vector<float>> out_sphe;

        // Color network 
        vector<vector<float>> out_cols = matrix_multiply(matrix_multiply(matrix_multiply(out_sphe, w1_color_, relu), w2_color_, relu), w3_color_, sigmoid); 
        
        //Compute final color
        /*
        if (exp(- accumulate_dens_) >= threshold || bidx == 7) {
            out_pixel = ComputeColor();
        } 
        */
       
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

vector<vector<float>> matrix_multiply(const vector<vector<float>>& A, const vector<vector<float>>& B, ActivationFunction activationFunc) {
    int m = A.size();
    int n = B[0].size();
    int p = B.size();    
    vector<vector<float>> result(m, vector<float>(n, 0.0));

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < p; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
            result[i][j] = activationFunc(result[i][j]);
        }
    }

    return result;
}

//ReLU activation function
float relu(float x) {
    return x > 0 ? x : 0; 
}

//sigmoid activation function (for low dynamic range: sRGB). Selected as default output actavtion.
float sigmoid(float x) {
    return 1.0f / (1.0f + exp(-x));
}

// ELU activation function (for high dynamic range: linear HDR)
float elu(float x, float alpha = 1.0f) {
    return x > 0 ? x : alpha * (exp(x) - 1);
}