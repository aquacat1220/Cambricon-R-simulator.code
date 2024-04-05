#include "SamplingUnit.hpp"

#include <cassert>

int main() {
    SamplingUnit sampling_unit = SamplingUnit(0.0, 100.0, 0.0, 100.0, 0.0, 100.0);

    sampling_unit.in_ray = vector<Ray>(); // Empty input.
    sampling_unit.Cycle();
    assert(sampling_unit.out_sample_batches.empty());
}