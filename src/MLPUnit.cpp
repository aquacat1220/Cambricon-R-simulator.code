#include "MLPUnit.hpp"

#include <cmath>
#include <cassert>
#include <boost/math/special_functions/spherical_harmonic.hpp>

using ActivationFunction = float (*)(float);

vector<vector<float>> matrix_multiply(const vector<vector<float>>& A, const vector<vector<float>>& B, ActivationFunction activationFunc);
float relu(float x);
float sigmoid(float x);
float elu(float x, float alpha = 1.0f);
vector<float> computeSphericalHarmonics(float theta, float phi);

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
        assert(out_dens.size() == 32);
        assert(out_dens[0].size() == 16);

        for (auto &out_den : out_dens) {
            densities_.push_back(exp(out_den[0]));
            accumulate_dens_ += exp(out_den[0]);
        }

        // Spherical harmonic
        // This part is not compeletly implemented.
        // Just concatenate output of density network and spherical harmonics basis values
        vector<float> sphe_basis = computeSphericalHarmonics(theta, phi);
        for (auto &out_den : out_dens) {
            out_den.insert(out_den.end(), sphe_basis.begin(), sphe_basis.end());
        }
        assert(out_dens[0].size() == 32);


        // Color network 
        vector<vector<float>> out_cols = matrix_multiply(matrix_multiply(matrix_multiply(out_dens, w1_color_, relu), w2_color_, relu), w3_color_, sigmoid); 
        colors_.insert(colors_.end(), out_cols.begin(), out_cols.end());

        //Compute final color
        if (exp( - accumulate_dens_ ) >= threshold || in_features[0].bidx == 7) {
            out_pixel = ComputeColor();
            this->ClearLocals();
        } 
        
        // Now clear inputs
        this->ClearInputs();

    } else {
        // if MLPUnit doesn't spend 711 cycles, just decrease remain_cycle_.
        if (remain_cycle_ > 0){
            remain_cycle_ -= 1;
        }
    }

}

void MlpUnit::MlpWeightLoad(vector<vector<float>>& w1_d, vector<vector<float>>& w2_d, vector<vector<float>>& w1_c, vector<vector<float>>& w2_c, vector<vector<float>>& w3_c) {
    w1_density_ = w1_d;
    w2_density_ = w2_d;
    w1_color_ = w1_c;
    w2_color_ = w2_c;
    w3_color_ = w3_c;
}

void MlpUnit::ClearInputs() {
    this->in_features.clear();
}

void MlpUnit::ClearLocals() {
    this->densities_.clear();
    this->colors_.clear();
    this->accumulate_dens_ = 0.0;
}

Pixel MlpUnit::ComputeColor() {
    Pixel pixel = {ridx, 0.0, 0.0, 0.0};
    float T = 1.0;
    
    for (size_t i = 0; i < colors_.size(); ++i) {
        float T_i = exp (- densities_[i] * distances_[i]);

        pixel.r += T * (1 - T_i) * colors_[i][0];
        pixel.g += T * (1 - T_i) * colors_[i][1];
        pixel.b += T * (1 - T_i) * colors_[i][2];

        T *= T_i;
    }
    return pixel;
}

const int& MlpUnit::GetRemainCycle() {
    const auto& ref = this->remain_cycle_;
    return ref;
}

void MlpUnit::SetDistance() {
    distances_.assign(256, 1.0f);
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
float elu(float x, float alpha) {
    return x > 0 ? x : alpha * (exp(x) - 1);
}

vector<float> computeSphericalHarmonics(float theta, float phi) {
    vector<float> Y;
    Y.reserve(16);

    // Iterate over the degrees and orders
    for (int l = 0; l <= 3; ++l) {
        for (int m = -l; m <= l; ++m) {
            double val = boost::math::spherical_harmonic_r(l, m, theta, phi);
            Y.push_back(val);
        }
    }

    return Y;
}