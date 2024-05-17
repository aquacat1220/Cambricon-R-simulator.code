#include "MLPUnit.hpp"

#include <cassert>
#include <algorithm>
#include <iostream>

vector<Feature> produce_feature_batch(unsigned int ridx, unsigned char bidx) {
    vector<Feature> out_feature_batch;
    for (unsigned char i=0; i <32; ++i){
        vector<float> floatvector(32, 1.0f);

        Feature feature = Feature {
            .ridx = ridx,
            .bidx = bidx,
            .pidx = i,
            .feature_vector = floatvector
        };
        out_feature_batch.push_back(feature);
    }
    return  out_feature_batch;
}

int main() {
    // Create MLPUnit
    MlpUnit mlpunit = MlpUnit(0);

    mlpunit.theta = 0.1f;
    mlpunit.phi = 0.1f;
    mlpunit.threshold = -1e8f;  //1e8f;
    mlpunit.SetDistance();
    
    unsigned int ridx_cnt = 0;
    unsigned char bidx_cnt = 0;
    mlpunit.in_features = produce_feature_batch(ridx_cnt, bidx_cnt);
    bidx_cnt++;

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

    for (int i=0; i<10000; ++i) {
        mlpunit.Cycle();
        if (mlpunit.is_idle() && bidx_cnt<=7) {
            if (mlpunit.GetHasOutput()) {
                //cout << "cycle" << i << ": " << mlpunit.out_pixel.ridx << endl;
                assert(mlpunit.out_pixel.ridx == ridx_cnt);
                ridx_cnt += 128;
                bidx_cnt = 0;
                mlpunit.in_features = produce_feature_batch(ridx_cnt, bidx_cnt);
                mlpunit.ridx = ridx_cnt;
            } else {
                mlpunit.in_features = produce_feature_batch(ridx_cnt, bidx_cnt);
                bidx_cnt = (bidx_cnt+1)%8;
                assert(!mlpunit.in_features.empty());
            }
        }
        
    }

    //cout << "is_idle(): " <<  mlpunit.is_idle() << endl;

    assert(!mlpunit.is_idle());
    assert(!mlpunit.in_features.empty());

    return 0;
}