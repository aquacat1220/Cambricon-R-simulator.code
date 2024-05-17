#include "MLPUnit.hpp"

#include <cassert>
#include <algorithm>

int main() {
    // Create MLPUnit
    MlpUnit mlpunit = MlpUnit(0);

    mlpunit.theta = 0.1f;
    mlpunit.phi = 0.1f;
    mlpunit.threshold = 1.0f;
    mlpunit.SetDistance();

    vector<vector<float>> w1_density(32, vector<float>(64, 1e-4));
    vector<vector<float>> w2_density(64, vector<float>(16, 1e-4));
    vector<vector<float>> w1_color(32, vector<float>(64, 1e-4));
    vector<vector<float>> w2_color(64, vector<float>(64, 1e-4));
    vector<vector<float>> w3_color(64, vector<float>(3, 1e-4));

    mlpunit.MlpWeightLoad(w1_density, w2_density, w1_color, w2_color, w3_color);

    auto before_in = mlpunit.in_features;

    sort(before_in.begin(), before_in.end());

    assert(before_in == vector<Feature>());

    for (int i=0; i<1000; ++i) {
        mlpunit.Cycle();
        assert(mlpunit.in_features.empty());
        assert(!mlpunit.GetHasOutput());
        assert(mlpunit.is_idle());
    }

    return 0;
}