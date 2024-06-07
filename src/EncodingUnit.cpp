#include "EncodingUnit.hpp"

#include <cmath>
#include <cassert>

#include "AIBACommon.hpp"

EncodingUnit::EncodingUnit(int min_grid_resolution, int max_grid_resolution) {
    float b = exp((log(max_grid_resolution) - log(min_grid_resolution)) / 15);
    for (int level = 0; level < 16; level++) {
        int grid_resolution = floor(min_grid_resolution * pow(b, level));
        grid_resolutions_.push_back(grid_resolution);
    }
    assert(grid_resolutions_.size() == 16);

    for (int grid_resolution : grid_resolutions_) {
        aiba_units_.emplace_back(grid_resolution);
    }
    assert(aiba_units_.size() == 16);
}

void EncodingUnit::Cycle() {
    this->was_idle_ = true;
    this->total_cycles_++;
    // Start by clearing previous output.
    this->ClearOutputs();

    // Convert Samples to Points. (Remove directional info)
    vector<Point> in_points;
    for (auto &in_sample : this->in_samples) {
        this->was_idle_ = false;
        in_points.emplace_back(in_sample.ridx, in_sample.bidx, in_sample.pidx, in_sample.x, in_sample.y, in_sample.z);
    }

    // Feed converted inputs to aiba units.
    for (auto &aiba : this->aiba_units_) {
        aiba.in_points = in_points;
    }

    // Cycle every aiba unit.
    for (auto &aiba : this->aiba_units_) {
        aiba.Cycle();
        this->was_idle_ = this->was_idle_ && aiba.WasIdle();
    }

    // Check outputs of every aiba unit, and accumulate into pfeature_buffer_.
    for (int level = 0; level < 16; level++) {
        auto out_sumss = this->aiba_units_[level].out_sums;
        for (auto out_sums : out_sumss) {
            this->was_idle_ = false;
            unsigned int ridx = out_sums[0].ridx;
            unsigned char bidx = out_sums[0].bidx;
            // If there is no matching entry, create a new one.
            if (!this->pfeature_buffer_.contains(make_pair(ridx, bidx))) {
                AddEmptyPFeatureVector(ridx, bidx);
            }
            // Accumulate the sums into the entry.
            AccumulteSumsToPFeatureVector(ridx, bidx, level, out_sums);
        }
    }

    // Now clear inputs.
    this->ClearInputs();
    if (this->was_idle_) {
        this->idle_cycles_++;
    }
}

void EncodingUnit::HashTableLoad(float* hash_table) {
    for (int level = 0; level < 16; level++) {
        aiba_units_[level].HashTableLoad(reinterpret_cast<HashEntry *>(hash_table + level * (1 << 20)));
    }
}

void EncodingUnit::ClearInputs() {
    this->in_samples.clear();
}

void EncodingUnit::ClearOutputs() {
    this->out_features.clear();
}

void EncodingUnit::AddEmptyPFeatureVector(unsigned int ridx, unsigned char bidx) {
    PFeature pfeature = { { ridx, bidx, 0, vector<float>(32, 0.0) }, 0 };
    vector<PFeature> pfeature_vec(32, pfeature);
    for (int pidx = 0; pidx < 32; pidx++) {
        pfeature_vec[pidx].feature.pidx = pidx;
    }
    this->pfeature_buffer_.insert({ make_pair(ridx, bidx), pfeature_vec });
}

void EncodingUnit::AccumulteSumsToPFeatureVector(unsigned int ridx, unsigned char bidx, int level, vector<Sum> out_sums) {
    // Find the pfeature vector in the buffer for accumulation.
    auto pfeature_vec_iter = this->pfeature_buffer_.find(make_pair(ridx, bidx));
    for (auto &out_sum : out_sums) {
        unsigned char pidx = out_sum.pidx;
        (*pfeature_vec_iter).second[pidx].feature.feature_vector[2*level] = out_sum.sum.elem0;
        (*pfeature_vec_iter).second[pidx].feature.feature_vector[2*level + 1] = out_sum.sum.elem1;

        (*pfeature_vec_iter).second[pidx].count++;
    }
    
    // If all 16 sums accumulated into the pfeatures, we can erase it from the buffer and pass it to the out_features.
    if ((*pfeature_vec_iter).second[0].count == 16) {
        vector<Feature> feature_vec;
        for (auto &pfeature : (*pfeature_vec_iter).second) {
            assert(pfeature.count == 16);
            feature_vec.push_back(pfeature.feature);
        }
        assert(feature_vec.size() == 32);
        this->out_features.push_back(feature_vec);
    }
}

bool EncodingUnit::WasIdle() {
    return this->was_idle_;
}

EncodingUnitStats EncodingUnit::GetStats() {
    vector<AIBAStats> aiba_statss;
    for (auto& aiba: this->aiba_units_) {
        aiba_statss.push_back(aiba.GetStats());
    }
    return EncodingUnitStats {
        .total_cycles = total_cycles_,
        .idle_cycles = idle_cycles_,
        .aiba_statss = aiba_statss
    };
}