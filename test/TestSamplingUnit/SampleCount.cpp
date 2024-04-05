#include "SamplingUnit.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>

int main() {
    SamplingUnit sampling_unit = SamplingUnit(0.0, 100.0, 0.0, 100.0, 0.0, 100.0);
    
    for (int i = 0; i < 256; i++) {
        // Ray starting at (10, 10, 10), pointing in the +x direction.
        Ray ray = {
            .ridx = (unsigned int)(i/2),
            .x = 10.0,
            .y = 10.0,
            .z = 10.0,
            .theta = 0,
            .phi = M_PI_2
        };
        if (i % 2 == 0) {
            sampling_unit.in_ray = vector<Ray>(1, ray);
        }
        sampling_unit.Cycle();
        vector<vector<Sample>> sample_batches = sampling_unit.out_sample_batches;

        if (i % 2 != 0) {
            continue;
        }

        assert(sample_batches.size() == 8); // 8 batches.
        for (auto& batch : sample_batches) {
            assert(batch.size() == 32); // 32 samples per batch.
        }

        for (int bidx = 0; bidx < 8; bidx++) {
            for (int pidx = 0; pidx < 32; pidx++) {
                assert(sample_batches[bidx][pidx].ridx == (unsigned int)(i/2));
                assert(sample_batches[bidx][pidx].bidx == bidx);
                assert(sample_batches[bidx][pidx].pidx == pidx);
            }
        }
    }
}