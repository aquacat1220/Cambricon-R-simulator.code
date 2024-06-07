#include "SamplingUnit.hpp"

#include <cmath>
#include <cassert>
#include <random>

void SamplingUnit::Cycle() {
    // Start with true, and change to false if any actions are observed.
    this->was_idle_ = true;
    this->total_cycles_++;
    // Start by clearing previous outputs.
    this->ClearOutputs();

    if (this->in_ray.empty()) { // If no ray was supplied to the unit, idle cycle!
        this->ClearInputs(); // Probably unnecessary, but just to be sure.
        if (this->was_idle_) {
            this->idle_cycles_++;
        }
        return;
    }

    this->was_idle_ = false;
    Ray in_ray = this->in_ray[0]; // Otherwise, process the ray.
    unsigned int ridx = in_ray.ridx;
    float x = in_ray.x;
    float y = in_ray.y;
    float z = in_ray.z;
    float theta = in_ray.theta;
    float phi = in_ray.phi;

    // Calculate the unit directional vector.
    float dir_x, dir_y, dir_z;
    dir_x = sin(theta) * cos(phi);
    dir_y = sin(theta) * sin(phi);
    dir_z = cos(theta);

    float t_min_x = fmin((this->min_x_ - x) / dir_x, (this->max_x_ - x) / dir_x);
    float t_max_x = fmax((this->min_x_ - x) / dir_x, (this->max_x_ - x) / dir_x);
    float t_min_y = fmin((this->min_y_ - y) / dir_y, (this->max_y_ - y) / dir_y);
    float t_max_y = fmax((this->min_y_ - y) / dir_y, (this->max_y_ - y) / dir_y);
    float t_min_z = fmin((this->min_z_ - z) / dir_z, (this->max_z_ - z) / dir_z);
    float t_max_z = fmax((this->min_z_ - z) / dir_z, (this->max_z_ - z) / dir_z);

    float t_min = fmax(0.0, fmax(t_min_x, fmax(t_min_y, t_min_z)));
    float t_max = fmin(t_max_x, fmin(t_max_y, t_max_z));

    // Assert fails if the ray does not intersect the bounding box.
    assert(t_min <= t_max);

    float dt = (t_max - t_min) / 256;

    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, dt);

    Sample empty_sample = { ridx, 0, 0, 0, 0, 0, theta, phi };
    vector<vector<Sample>> out_sample_batches(8, vector<Sample>(32, empty_sample));

    for (int bidx = 0; bidx < 8; bidx++) {
        for (int pidx = 0; pidx < 32; pidx++) {
            float t = t_min + dt * (32 * bidx + pidx) + dis(gen);
            out_sample_batches[bidx][pidx].bidx = bidx;
            out_sample_batches[bidx][pidx].pidx = pidx;
            out_sample_batches[bidx][pidx].x = x + dir_x * t;
            out_sample_batches[bidx][pidx].y = y + dir_y * t;
            out_sample_batches[bidx][pidx].z = z + dir_z * t;
        }
    }
    this->out_sample_batches = out_sample_batches;

    this->ClearInputs();
    if (this->was_idle_) {
        this->idle_cycles_++;
    }
    return;
}

void SamplingUnit::ClearInputs() {
    this->in_ray.clear();
}

void SamplingUnit::ClearOutputs() {
    this->out_sample_batches.clear();
}

bool SamplingUnit::WasIdle() {
    return this->was_idle_;
}

SamplingUnitStats SamplingUnit::GetStats() {
    return SamplingUnitStats {
        .total_cycles = total_cycles_,
        .idle_cycles = idle_cycles_
    };
}