#include "CambR.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>
#include <iostream>

vector<Ray> create_rays(mt19937 rand_engine, uniform_real_distribution<>& real_dist) {
    vector<Ray> rays;
    rays.reserve(256);

    for (unsigned int i = 0; i < 256; ++i) {
        rays.push_back(Ray{
            .ridx = i,
            .x = 1.0f,
            .y = 1.0f,
            .z = 1.0f,
            .theta = static_cast<float>(real_dist(rand_engine)),
            .phi = static_cast<float>(real_dist(rand_engine))
        });
    }

    assert(rays.size() == 256);

    return rays;
}

int main() {
    
    //cout << "Start test1!" << endl;
    vector<float> hash_table(1 << 24, 1.0f);

    //cout << "Start test2!" << endl;
    vector<vector<float>> w1_density(32, vector<float>(64, 1e-4));
    vector<vector<float>> w2_density(64, vector<float>(16, 1e-4));
    vector<vector<float>> w1_color(32, vector<float>(64, 1e-4));
    vector<vector<float>> w2_color(64, vector<float>(64, 1e-4));
    vector<vector<float>> w3_color(64, vector<float>(3, 1e-4));

    //cout << "Start test3!" << endl;

    CambR cambr = CambR(0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 1<<4 ,1<<12, &hash_table.front(), w1_density, w2_density, w1_color, w2_color, w3_color);

    //cout << "Construct CambR!" << endl;

    assert(cambr.in_rays.empty());
    assert(cambr.out_pixels.empty());

    mt19937 rand_engine(20240515);
    uniform_real_distribution<> real_dist(0.0, M_PI_2);

    cambr.in_rays = create_rays(rand_engine, real_dist);

    //cout << "Create input rays!" << endl;

    int elapsed_cycles = 0;
    int num_total_out_pixels = 0;
    vector<Pixel> out_pixels;

    while (num_total_out_pixels != 256) {
        cambr.Cycle();
        //cout << elapsed_cycles << endl;
        num_total_out_pixels += cambr.out_pixels.size();
        out_pixels.insert(out_pixels.end(), cambr.out_pixels.begin(), cambr.out_pixels.end());
        elapsed_cycles++;
    }

    //cout << "End Cycles()!" << endl;
    //cout << num_total_out_pixels <<endl;
    //cout << out_pixels.size() << endl;

    assert(cambr.in_rays.empty());
    assert(!cambr.out_pixels.empty());
    assert(num_total_out_pixels == out_pixels.size());

    //cout << "-------------------" << endl;

    sort(out_pixels.begin(), out_pixels.end());

    unsigned int pixel_count = 0;
    for (auto& out_pixel : out_pixels) {
        //cout << out_pixel.ridx << endl;
        //assert(out_pixel.ridx == pixel_count);
        pixel_count++;
    }

    //cout << "successful Out_pixels!" << endl;
    
    return 0;
}