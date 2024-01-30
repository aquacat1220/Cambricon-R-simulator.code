#include "AIBAPP.hpp"

#include <tuple>

AIBAPP::AIBAPP(int grid_resolution) {
    this->grid_resolution = grid_resolution;
    this->dest_reg = 0;
    this->ridx_reg = 0;
}

void AIBAPP::Cycle() {
    // Clear all outputs before generating new ones.
    this->ClearOutputs();

    // Iterate over all 32 input points and produce requests.
    for (unsigned char pidx = 0; pidx < 32; pidx++) {
        Point& pt = in_point_batch[pidx];

        unsigned int gridx, gridy, gridz;
        gridx = static_cast<int>(pt.x * this->grid_resolution);
        gridy = static_cast<int>(pt.y * this->grid_resolution);
        gridz = static_cast<int>(pt.z * this->grid_resolution);

        // Structured bindings + tuple in C++!! So cool.
        tuple<int, int, int> offsets[8] = {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};
        for (auto [offx, offy, offz] : offsets) {
            gridx = gridx + offx;
            gridy = gridy + offy;
            gridz = gridz + offz;
            unsigned int addr = Hash(gridx, gridy, gridz) % (2 << 19);
            
            // @todo Implement trilinear interpolation to calculate weight.
            float weight;

            out_reqs.emplace_back(this->ridx_reg, pidx, addr, weight, this->dest_reg + pidx);
        }
    }
    return;
}

void AIBAPP::ClearOutputs() {
    out_reqs.clear();
}

unsigned int AIBAPP::Hash(int gridx, int gridy, int gridz) {
    return (gridx * 1) ^ (gridy * 2654435761) ^ (gridz * 805459861);
}