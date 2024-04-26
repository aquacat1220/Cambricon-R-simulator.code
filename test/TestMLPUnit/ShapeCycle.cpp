#include "MLPUnit.hpp"

#include <cassert>
#include <iostream>

int main() {
    /*
    MlpUnit mlpunit = MlpUnit();
    
    // Set input for MlpUnit
    for (unsigned char i=0 ; i <32; ++i){
        vector<float> floatvector(32, 1.0f);

        Feature feature = Feature {
            .ridx = 1,
            .bidx = 0,
            .pidx = i,
            .feature_vector = floatvector
        };

         mlpunit.in_features.push_back(feature);
    }

    assert(mlpunit.in_features.size() == 32);

    // Check Whether output is Null value
    auto before_out = mlpunit.out_pixel;
    assert(!before_out.ridx);

    // Set Some members needed to compute MLP
    mlpunit.ridx = 1;
    mlpunit.theta = 1.0f;
    mlpunit.phi = 1.0f;
    mlpunit.threshold = 0.0f;
    mlpunit.SetDistance();

    // Load weight matrices
    // (32x64, 64x16 for density network & 32x64, 64x64, 64x3 for color network)
    vector<vector<float>> w1_density(32, vector<float>(64, 1e-4));
    vector<vector<float>> w2_density(64, vector<float>(16, 1e-4));
    vector<vector<float>> w1_color(32, vector<float>(64, 1e-4));
    vector<vector<float>> w2_color(64, vector<float>(64, 1e-4));
    vector<vector<float>> w3_color(64, vector<float>(3, 1e-4));
    mlpunit.MlpWeightLoad(w1_density, w2_density, w1_color, w2_color, w3_color);

    cout << "start MLP computation!" << endl;
    cout << "outpixel's ridx before a cycle() : " << mlpunit.out_pixel.ridx << endl;
    cout << "remain_cycle before a cycle() : " << mlpunit.GetRemainCycle() <<endl;
    // Compute 1 Cycle
    mlpunit.Cycle();

    cout << "outpixel's ridx after a cycle() : " << mlpunit.out_pixel.ridx << endl;
    cout << "remain_cycle after a cycle() : " << mlpunit.GetRemainCycle() <<endl;
    assert(mlpunit.out_pixel.ridx == 1);

    // Compute 710 Cycle
    for (int i = 0; i < 710; ++i) {
        mlpunit.Cycle();
        assert(mlpunit.GetRemainCycle() == (709-i));
    }
    cout << "remain_cycle after 711 cycle()s : " << mlpunit.GetRemainCycle() <<endl;
    */

    return 0;
   
}