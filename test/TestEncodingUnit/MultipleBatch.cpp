#include "EncodingUnit.hpp"

#include <vector>
#include <cassert>
#include <random>
#include <cmath>
#include <iostream>

vector<Sample> produce_sample_batch(unsigned char ridx, unsigned char bidx, mt19937 rand_engine, uniform_real_distribution<> real_dist) {
    vector<Sample> in_samples(32, {
        .ridx = ridx,
        .bidx = bidx,
        .pidx = 0,
        .x = 0,
        .y = 0,
        .z = 0,
        .theta = real_dist(rand_engine),
        .phi = real_dist(rand_engine),
    });
    
    for (int i = 0; i < 32; i++) {
        in_samples[i].pidx = i;
        in_samples[i].x = real_dist(rand_engine);
        in_samples[i].y = real_dist(rand_engine);
        in_samples[i].z = real_dist(rand_engine);
    }

    return in_samples;
}

int main() {
    EncodingUnit encoding_unit = EncodingUnit(16, 4096);

    vector<float> hash_table(1 << 24, 1.0f);
    encoding_unit.HashTableLoad(&hash_table.front());

    assert(encoding_unit.in_samples.empty());

    mt19937 rand_engine(20240510);
    uniform_real_distribution<> real_dist(0.0, M_PI_2);

    int elapsed_cycles = 0;
    unsigned char bidx = 0;
    int out_feature_batch_count = 0;
    vector<vector<Feature>> out_feature_batches;

    while (out_feature_batch_count < 32) {
        if (elapsed_cycles % 3 == 0 && bidx < 32) {
            cout << "Cycle: " << elapsed_cycles << "| bidx: " << (int)bidx << " to input." << endl;
            encoding_unit.in_samples = produce_sample_batch(0, bidx++, rand_engine, real_dist); 
        }

        encoding_unit.Cycle();

        for (auto& out_feature_batch : encoding_unit.out_features) {
            cout << "Cycle: " << elapsed_cycles << "| bidx: " << (int)out_feature_batch[0].bidx << " from output." << endl;
            out_feature_batches.push_back(out_feature_batch);
            out_feature_batch_count++;
        }

        elapsed_cycles++;
    }

    assert(out_feature_batches.size() == 32);
    vector<vector<Feature>> sorted_out_feature_batches(32, { 32, { .ridx = 1, .bidx = 0, .pidx = 0, .feature_vector = { 0.0f } }});
    for (auto& out_feature_batch : out_feature_batches) {
        sorted_out_feature_batches[out_feature_batch[0].bidx] = out_feature_batch;
    }
    for (int i = 0; i < 32; i++) {
        vector<Feature> out_feature_batch = sorted_out_feature_batches[i];
        for (int j = 0; j < 32; j++) {
            assert(out_feature_batch[j].ridx != 1);
            assert(out_feature_batch[j].bidx == i);
            assert(out_feature_batch[j].pidx == j);
            assert(out_feature_batch[j].feature_vector.size() == 32);
            for (int k = 0; k < 32; k++) {
                assert(abs(out_feature_batch[j].feature_vector[k] - 1.0f) < 0.0001f);
            }
        }
    }
}