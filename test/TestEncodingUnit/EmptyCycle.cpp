#include "EncodingUnit.hpp"

#include <vector>
#include <cassert>

int main() {
    EncodingUnit encoding_unit = EncodingUnit(16, 4096);

    vector<float> hash_table(1 << 24, 1.0f);
    encoding_unit.HashTableLoad(&hash_table.front());

    for (int i = 0; i < 256; i++) {
        assert(encoding_unit.in_samples.empty());
        encoding_unit.Cycle();
        assert(encoding_unit.out_features.empty());
    }
}