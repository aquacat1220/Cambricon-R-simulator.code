#include "MLPUnit.hpp"

#include <cassert>
#include <algorithm>
#include <iostream>

int main() {
    // Create MLPUnit
    MlpUnit mlpunit = MlpUnit(0);

    mlpunit.theta = 0.1f;
    mlpunit.phi = 0.1f;
    mlpunit.threshold = -1e8f;
    mlpunit.SetDistance();

    //create single batch
    for (unsigned char i=0 ; i <32; ++i){
        vector<float> floatvector(32, 1.0f);

        Feature feature = Feature {
            .ridx = 0,
            .bidx = 0,
            .pidx = i,
            .feature_vector = floatvector
        };

         mlpunit.in_features.push_back(feature);
    }

    auto before_in = mlpunit.in_features;

    sort(before_in.begin(), before_in.end());

    assert(before_in.begin()->pidx == 0);
    assert(before_in.size() == 32);

    //Load weight
    vector<vector<float>> w1_density(32, vector<float>(64, 1e-0));
    vector<vector<float>> w2_density(64, vector<float>(16, 1e-0));
    vector<vector<float>> w1_color(32, vector<float>(64, 1e-0));
    vector<vector<float>> w2_color(64, vector<float>(64, 1e-0));
    vector<vector<float>> w3_color(64, vector<float>(3, 1e-0));

    mlpunit.MlpWeightLoad(w1_density, w2_density, w1_color, w2_color, w3_color);

    for (int i=0; i<1000; ++i) {
        mlpunit.Cycle();
        
        //cout << i << ": " << mlpunit.GetHasOutput() << endl;    (print 1 when i == 710, 0 otherwise)
        //cout << mlpunit.in_features.empty() << endl;
        //cout << mlpunit.is_idle() << endl;
    }

    return 0;
}