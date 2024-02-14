#include "AIBAPP.hpp"

#include <algorithm>
#include <cassert>

int main() {
    // Create AIBA preprocessor unit with grid resolution 2^12.
    AIBAPP pp = AIBAPP(1 << 12);

    auto before_in = pp.in_point_batch;
    auto before_out = pp.out_reqs;

    sort(before_in.begin(), before_in.end());
    sort(before_out.begin(), before_out.end());

    assert(before_in == vector<Point>());
    assert(before_out == vector<Request>());

    return 0;
}