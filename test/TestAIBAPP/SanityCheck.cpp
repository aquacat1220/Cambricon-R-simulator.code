#include "AIBAPP.hpp"

#include <algorithm>
#include <cassert>
#include <random>
#include <tuple>
#include <iostream>

/**
 * @brief Randomly generates 32 Points and their matching 256 Requests.
 * 
 * @param rand_engine Random engine to use.
 * @param real_dist Distribution of real numbers.
 * @return tuple<vector<Point>, vector<Request>> Tuple of 32 Point vector and 256 Request vector.
 */
tuple<vector<Point>, vector<Request>> GeneratePoints(mt19937 rand_engine, uniform_real_distribution<> real_dist);

// Generate random points and feed it into AIBAPP.
int main() {
    // Create AIBA preprocessor unit with grid resolution 2^12.
    AIBAPP pp = AIBAPP(1 << 12);

    // Seed the random engine with a set seed to be deterministic.
    mt19937 rand_engine(20240206);
    uniform_real_distribution<> real_dist(0.0, 1024.0);

    // Try 1024 cycles.
    for (int i = 0; i < 1024; i++) {
        // Input vector should be empty.
        auto before_in = pp.in_point_batch;
        sort(before_in.begin(), before_in.end());
        assert(before_in == vector<Point>());

        auto [in_point_batch, expected_out_reqs] = GeneratePoints(rand_engine, real_dist);

        pp.in_point_batch = in_point_batch;
        pp.Cycle();

        auto after_out_reqs = pp.out_reqs;
        // sort(after_out_reqs.begin(), after_out_reqs.end());
        // sort(expected_out_reqs.begin(), expected_out_reqs.end());
        for (int i = 0; i < 256; i++) {
            auto &after_out_req = after_out_reqs[i];
            auto &expected_out_req = expected_out_reqs[i];
            if (after_out_req != expected_out_req) {
                cout << "after_out_req = { " << "ridx: " << (unsigned int)after_out_req.ridx << ", " << "pidx: " << (unsigned int)after_out_req.pidx << ", " << \
                "addr: " << after_out_req.addr << ", " << "weight: " << after_out_req.weight << ", " << "dest: " << (unsigned int)after_out_req.dest << " }" << endl;
                cout << "expected_out_req = { " << "ridx: " << (unsigned int)expected_out_req.ridx << ", " << "pidx: " << (unsigned int)expected_out_req.pidx << ", " << \
                "addr: " << expected_out_req.addr << ", " << "weight: " << expected_out_req.weight << ", " << "dest: " << (unsigned int)expected_out_req.dest << " }" << endl;
            }
        }
        assert(after_out_reqs == expected_out_reqs);
    }

    return 0;
}

tuple<vector<Point>, vector<Request>> GeneratePoints(mt19937 rand_engine, uniform_real_distribution<> real_dist) {
    static unsigned char ridx_reg = 0;
    static unsigned char dest_reg = 0;

    // Preallocate memory for return vectors.
    vector<Point> points;
    points.reserve(32);
    vector<Request> reqs;
    reqs.reserve(256);

    // Make one Point / eight Requests at a time.
    for (int pidx = 0; pidx < 32; pidx++) {
        float x = real_dist(rand_engine);
        float y = real_dist(rand_engine);
        float z = real_dist(rand_engine);
        int grid_x = x * (1 << 12);
        int grid_y = y * (1 << 12);
        int grid_z = z * (1 << 12);
        float offx = x * (1 << 12) - grid_x;
        float offy = y * (1 << 12) - grid_y;
        float offz = z * (1 << 12) - grid_z;

        points.emplace_back(x, y, z);

        tuple<int, int, int> grid_offsets[8] = {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};
        for (auto [grid_offx, grid_offy, grid_offz] : grid_offsets) {
            unsigned int addr = ((grid_x + grid_offx) * 1) ^ ((grid_y + grid_offy) * 2654435761) ^ ((grid_z + grid_offz) * 805459861);
            addr = addr % (1 << 19);
            float weight = abs(grid_offx - offx) * abs(grid_offy - offy) * abs(grid_offz - offz);

            reqs.emplace_back(ridx_reg, pidx, addr, weight, dest_reg + pidx);
        }
    }

    ridx_reg++;
    dest_reg += 32;
    return make_tuple(points, reqs);
}