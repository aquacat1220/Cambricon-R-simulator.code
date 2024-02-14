#include "AIBA.hpp"

#include <iostream>
#include <cassert>

AIBA::AIBA(int grid_resolution) : AIBAPP_(grid_resolution) {
    // Initialize AIBANode mesh.
    this->AIBANodes_.reserve(16 * 16);
    for (unsigned int coord = 0; coord < 256; coord++) {
        AIBANodes_.emplace_back(static_cast<unsigned char>(coord));
    }

    if (AIBANodes_.size() != 256)
        cout << ("[ERROR] AIBA::AIBANode_.size() is not 256.") << endl;
}

void AIBA::Cycle(){
    ClearOutputs();
    // Preprocessing by AIBAPP
    AIBAPP_.in_point_batch = in_points;
    AIBAPP_.Cycle();

    // 16x16 AIBANode mesh Cycle()
    for (AIBANode &aibanode : AIBANodes_) {
        aibanode.Cycle();
    }

    this->Transition();
    ClearInputs();
}

void AIBA::Transition(){
    //address crossbar
    for (Request &req : AIBAPP_.out_reqs) {
        unsigned char target_row = static_cast<unsigned char> ((req.addr >> 15) & 0x0F);
        AIBANodes_[target_row * 16].in_left_reqs.push_back(req);
    }

    // Propagate outputs from each node to appropriate inputs.
    for (int i = 0; i < 256; i++) {
        NodeTransition(static_cast<unsigned char>(i));
    }
}

void AIBA::ClearInputs() {
    this->in_points.clear();
}

void AIBA::ClearOutputs() {
    this->out_sums.clear();
}

void AIBA::NodeTransition(unsigned char coord) {
    unsigned char row = coord >> 4;
    unsigned char col = coord & 0xF;
    auto &node = this->AIBANodes_[coord];

    // Propagate Requests.
    if (!node.out_right_reqs.empty()) {
        // 15th column doesn't have a right node to propagate requests. Something must be wrong.
        assert(col != 15);

        auto &right_node = this->AIBANodes_[(row << 4) + (col + 1)];
        right_node.in_left_reqs = node.out_right_reqs;
    }

    // Propagate Packets.
    if (!node.out_left_pkts.empty()) {
        // 0th column doesn't have a left node to propagate packets. Something must be wrong.
        assert(col != 0);

        auto &left_node = this->AIBANodes_[(row << 4) + (col - 1)];
        left_node.in_right_pkts = node.out_left_pkts;
    }
    if (!node.out_right_pkts.empty()) {
        // 15th column doesn't have a right node to propagate packets. Something must be wrong.
        assert(col != 15);

        auto &right_node = this->AIBANodes_[(row << 4) + (col + 1)];
        right_node.in_left_pkts = node.out_right_pkts;
    }
    if (!node.out_up_pkts.empty()) {
        // 0th row doesn't have a up node to propagate packets. Something must be wrong.
        assert(row != 0);

        auto &up_node = this->AIBANodes_[((row - 1) << 4) + (col)];
        up_node.in_down_pkts = node.out_up_pkts;
    }
    if (!node.out_down_pkts.empty()) {
        // 15th row doesn't have a down node to propagate packets. Something must be wrong.
        assert(row != 15);

        auto &down_node = this->AIBANodes_[((row + 1) << 4) + (col)];
        down_node.in_up_pkts = node.out_down_pkts;
    }

    // Propagate Sums.
    if (!node.out_down_sums.empty()) {
        // Not bottommost node: do trivial stuff.
        if (row != 15) {
            auto &down_node = this->AIBANodes_[((row + 1) << 4) + (col)];
            down_node.in_up_sums = node.out_down_sums;
        }

        // Bottommost node: insert to multimap.
        else {
            for (auto &out_sum : node.out_down_sums) {
                this->sum_buffer_.insert(make_pair(out_sum.ridx, out_sum));
                CheckSumBuffer(out_sum.ridx);
            }
        }
    }
}

void AIBA::CheckSumBuffer(unsigned char ridx) {
    if (this->sum_buffer_.count(ridx) != 32) {
        // Not ready.
        return;
    }
    vector<Sum> sum_vec;
    auto its = this->sum_buffer_.equal_range(ridx);
    for (auto it = its.first; it != its.second; it++) {
        sum_vec.push_back(it->second);
    }
    assert(sum_vec.size() == 32);

    this->out_sums.push_back(sum_vec);
    this->sum_buffer_.erase(ridx);
    return;
}