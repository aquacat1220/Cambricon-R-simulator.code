#include "AIBA.hpp"

#include <algorithm>
#include <cassert>

int main() {
    // Create AIBA (AIBA preprocessor unit with 2^12 grid resolution.)
    AIBA aiba = AIBA(1 << 12);

    vector<HashEntry> orig_hash_table;
    orig_hash_table.assign((1 << 19),{ 1.0f, 1.0f });
    aiba.HashTableLoad(&orig_hash_table.front());

    for (int i = 0; i < 256; i++) {
        aiba.Cycle();
    }

    assert(aiba.out_sums.empty());

    return 0;
}