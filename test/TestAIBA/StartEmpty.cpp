#include "AIBA.hpp"

#include <algorithm>
#include <cassert>

int main() {
    // Create AIBA (AIBA preprocessor unit with 2^12 grid resolution.)
    AIBA aiba = AIBA(1 << 12);

    auto before_in = aiba.in_points;
    auto before_out = aiba.out_sum_buffer;

    sort(before_in.begin(), before_in.end());

    assert(before_in == vector<Point>());
    assert(before_out.empty());

    return 0;
}