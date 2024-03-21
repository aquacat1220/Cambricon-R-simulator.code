#include "SamplingUnit.hpp"

#include <cmath>
#include <cassert>
#include <random>

void SamplingUnit::Cycle() {
    // Start by clearing previous outputs.
    this->ClearOutputs();

    if (this->in_ray.empty()) { // If no ray was supplied to the unit, idle cycle!
        this->ClearInputs(); // Probably unnecessary, but just to be sure.
        return;
    }

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

    float t_min_x = (this->min_x - x) / dir_x;
    float t_max_x = (this->max_x - x) / dir_x;
    float t_min_y = (this->min_y - y) / dir_y;
    float t_max_y = (this->max_y - y) / dir_y;
    float t_min_z = (this->min_z - z) / dir_z;
    float t_max_z = (this->max_z - z) / dir_z;

    if (t_min_x > t_max_x) {
        float tmp = t_min_x;
        t_min_x = t_max_x;
        t_max_x = tmp;
    }

    if (t_min_y > t_max_y) {
        float tmp = t_min_y;
        t_min_y = t_max_y;
        t_max_y = tmp;
    }

    if (t_min_z > t_max_z) {
        float tmp = t_min_z;
        t_min_z = t_max_z;
        t_max_z = tmp;
    }

    float t_min = fmax(t_min_x, fmax(t_min_y, t_min_z));
    t_min = (t_min < 0) ? 0 : t_min;
    float t_max = fmin(t_max_x, fmin(t_max_y, t_max_z));
    t_max = (t_max < 0) ? 0 : t_max;

    // Assert fails if the ray does not intersect the bounding box.
    assert(t_min <= t_max);

    float dt = (t_min - t_max) / 256;

    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, dt);

    Sample empty_sample = { ridx, 0, 0, 0, 0, 0, theta, phi };
    vector<vector<Sample>> out_sample_batches(8, vector<Sample>(32, empty_sample));

    // Generate a random float in the range [0, t]
    float random_float = dis(gen);

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

    this->ClearInputs();
    return;
}

void SamplingUnit::ClearInputs() {
    this->in_ray.clear();
}

void SamplingUnit::ClearOutputs() {
    this->out_sample_batches.clear();
}