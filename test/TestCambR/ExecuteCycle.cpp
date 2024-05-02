#include "CambR.hpp"

#include <algorithm>
#include <cassert>

Ray createRay(unsigned int ridx) {
    return Ray{
        ridx,              // ridx as index
        1.0f,       // Example x value
        1.0f,       // Example y value
        1.0f,       // Example z value
        ridx * 0.01f,      // Example theta value
        ridx * 0.02f       // Example phi value
    };
}

int main() {
    unordered_map<unsigned int, Ray> rays;

    for (unsigned int i = 0; i < 64; ++i) {
        rays[i] = createRay(i);
    }

    vector<HashEntry> orig_hash_table;
    orig_hash_table.assign(1 << 23, {1.0f, 1.0f});

    vector<vector<float>> w1_density(32, vector<float>(64, 1e-4));
    vector<vector<float>> w2_density(64, vector<float>(16, 1e-4));
    vector<vector<float>> w1_color(32, vector<float>(64, 1e-4));
    vector<vector<float>> w2_color(64, vector<float>(64, 1e-4));
    vector<vector<float>> w3_color(64, vector<float>(3, 1e-4));


    CambR cambr = CambR(0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 1<<4 ,1<<9, &rays, &orig_hash_table.front(), w1_density, w2_density, w1_color, w2_color, w3_color);

    for(int i=0; i < (1<<10); ++i) {
        cambr.Cycle();
    }

    return 0;
}