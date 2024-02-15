#include "AIBAPP.hpp"

#include <tuple>

AIBAPP::AIBAPP(int grid_resolution) {
    this->grid_resolution_ = grid_resolution;
    this->dest_reg_ = 0;
}

void AIBAPP::Cycle() {
    // Clear all outputs before generating new ones.
    this->ClearOutputs();

    // Iterate over all 32 input points and produce requests.
    for (unsigned char pidx = 0; pidx < 32; pidx++) {
        Point& pt = in_point_batch[pidx];

        unsigned int grid_x, grid_y, grid_z;
        grid_x = static_cast<int>(pt.x * this->grid_resolution_);
        grid_y = static_cast<int>(pt.y * this->grid_resolution_);
        grid_z = static_cast<int>(pt.z * this->grid_resolution_);

        tuple<int, int, int> grid_offsets[8] = {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};

        auto weights = TriLerp((pt.x * this->grid_resolution_) - grid_x, (pt.y * this->grid_resolution_) - grid_y, (pt.z * this->grid_resolution_) - grid_z);

        for (int i = 0; i < 8; i++) {
            auto [grid_offx, grid_offy, grid_offz] = grid_offsets[i];
            float weight = weights[i];
            unsigned int addr = Hash(grid_x + grid_offx, grid_y + grid_offy, grid_z + grid_offz) % (1 << 19);

            out_reqs.emplace_back(pt.ridx, pt.bidx, pt.pidx, addr, weight, this->dest_reg_ + pidx);
        }
    }

    // Increment dest register.
    this->dest_reg_ += 32;
    
    // Clear all inputs before returning.
    this->ClearInputs();
    return;
}

void AIBAPP::ClearInputs() {
    this->in_point_batch.clear();
}

void AIBAPP::ClearOutputs() {
    this->out_reqs.clear();
}

unsigned int AIBAPP::Hash(int grid_x, int grid_y, int grid_z) {
    return (grid_x * 1) ^ (grid_y * 2654435761) ^ (grid_z * 805459861);
}

vector<float> AIBAPP::TriLerp(float offx, float offy, float offz) {
    vector<float> return_vec;
    return_vec.reserve(8);
    tuple<int, int, int> grid_offsets[8] = {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};
    for (auto [grid_offx, grid_offy, grid_offz] : grid_offsets) {
        return_vec.push_back(abs((grid_offx - offx) * (grid_offy - offy) * (grid_offz - offz)));
    }
    return return_vec;
}