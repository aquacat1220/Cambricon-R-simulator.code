#include "SamplingUnit.hpp"
#include "EncodingUnit.hpp"

#include <cassert>

int main() {
    EncodingUnit encoding_unit = EncodingUnit(16, 4096);

    for (int i = 0; i < 256; i++) {
        assert(encoding_unit.in_samples.empty());
        encoding_unit.Cycle();
        assert(encoding_unit.out_features.empty());
    }
}