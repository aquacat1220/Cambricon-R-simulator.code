#include "SamplingUnit.hpp"

#include <random>
#include <cassert>
#include <cmath>
#include <cstdio>

int main() {
    SamplingUnit sampling_unit = SamplingUnit(0.0, 100.0, 0.0, 100.0, 0.0, 100.0);

    random_device rd;
    mt19937 gen(rd());
    
    // Define the distribution
    uniform_real_distribution<float> point_dist(0.0, 100.0);
    uniform_real_distribution<float> angle_dist(0.0, M_2_PI);
    
    // Rays with origin in bounding box.
    for (int i = 0; i < 256; i++) {
        // Ray starting at (10, 10, 10), pointing in the +x direction.
        Ray ray = {
            .ridx = (unsigned int)(i),
            .x = point_dist(gen),
            .y = point_dist(gen),
            .z = point_dist(gen),
            .theta = angle_dist(gen),
            .phi = angle_dist(gen)
        };
        sampling_unit.in_ray = vector<Ray>(1, ray);
        sampling_unit.Cycle();
        vector<vector<Sample>> sample_batches = sampling_unit.out_sample_batches;

        for (int bidx = 0; bidx < 8; bidx++) {
            for (int pidx = 0; pidx < 32; pidx++) {
                const Sample& sample = sample_batches[bidx][pidx];
                assert(0.0 <= sample.x && sample.x <= 100.0);
                assert(0.0 <= sample.y && sample.y <= 100.0);
                assert(0.0 <= sample.z && sample.z <= 100.0);
            }
        }
    }

    uniform_real_distribution<float> point_dist2(-25.0, 0.0);
    uniform_real_distribution<float> angle_dist2(M_PI/6.0, M_PI/3.0);

    // Rays with origin in bounding box.
    for (int i = 256; i < 512; i++) {
        // Ray starting at (10, 10, 10), pointing in the +x direction.
        Ray ray = {
            .ridx = (unsigned int)(i),
            .x = point_dist2(gen),
            .y = point_dist2(gen),
            .z = point_dist2(gen),
            .theta = angle_dist2(gen),
            .phi = angle_dist2(gen)
        };
        sampling_unit.in_ray = vector<Ray>(1, ray);
        sampling_unit.Cycle();
        vector<vector<Sample>> sample_batches = sampling_unit.out_sample_batches;

        for (int bidx = 0; bidx < 8; bidx++) {
            for (int pidx = 0; pidx < 32; pidx++) {
                const Sample& sample = sample_batches[bidx][pidx];
                assert(0.0 <= sample.x && sample.x <= 100.0);
                assert(0.0 <= sample.y && sample.y <= 100.0);
                assert(0.0 <= sample.z && sample.z <= 100.0);
            }
        }
    }
}