#include "EncodingUnit.hpp"

#include <vector>
#include <cassert>

int main() {
    EncodingUnit encoding_unit = EncodingUnit(16, 4096);

    vector<float> hash_table(1 << 24, 1.0f);
    encoding_unit.HashTableLoad(&hash_table.front());

    assert(encoding_unit.in_samples.empty());

    vector<Sample> in_samples(32, {
        .ridx = 0,
        .bidx = 0,
        .pidx = 0,
        .x = 0,
        .y = 0,
        .z = 0,
        .theta = 0,
        .phi = 0,
    });
    
    for (int i = 0; i < 32; i++) {
        in_samples[i].pidx = i;
    }

    encoding_unit.in_samples = in_samples;
    do {
        encoding_unit.Cycle();
    } while (encoding_unit.out_features.empty());

    vector<vector<Feature>> out_features =  encoding_unit.out_features;
    assert(out_features.size() == 1);
    vector<Feature> out_feature_batch = out_features[0];
    assert(out_feature_batch.size() == 32);
    for (int i = 0; i < 32; i++) {
        Feature feature = out_feature_batch[i];
        assert(feature.ridx == 0);
        assert(feature.bidx == 0);
        assert(feature.pidx == i);
        assert(feature.feature_vector.size() == 32);
        for (int j = 0; j < 32; j++) {
            assert(feature.feature_vector[j] == 1.0f);
        }
    }
}