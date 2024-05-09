#include "System.hpp"

#include <iostream>

System::System(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z, 
            int min_grid_resolution, int max_grid_resolution,
            float* hash_table,
			vector<vector<float>>& w1_d, vector<vector<float>>& w2_d, vector<vector<float>>& w1_c, vector<vector<float>>& w2_c, vector<vector<float>>& w3_c) : camb_r_(min_x, max_x, min_y, max_y, min_z, max_z, min_grid_resolution, max_grid_resolution, hash_table, w1_d, w2_d, w1_c, w2_c, w3_c){
};

void System::request(vector<SystemRay> rays) {
    for (auto ray : rays) {
        this->camb_r_.in_rays.emplace_back(next_ridx_++, ray.x, ray.y, ray.z, ray.theta, ray.phi);
    };
};

vector<SystemPixel> System::handle_requests() {
    int pixels_left = this->next_ridx_ - this->last_ridx_ - 1;
    int elapsed_cycles = 0;
    cout << "Handling request of " << pixels_left << " rays!" << endl;

    vector<SystemPixel> out_pixels(pixels_left);
    
    while (pixels_left != 0) {
        this->camb_r_.Cycle();
        for (Pixel pixel : this->camb_r_.out_pixels) {
            out_pixels[pixel.ridx - this->last_ridx_ - 1] = { .r = pixel.r, .g = pixel.g, .b = pixel.b };
            pixels_left--;
        }
        this->cycle_count_++;
        elapsed_cycles++;
    }

    this->last_ridx_ = this->next_ridx_ - 1;

    cout << "Request handled! - " << elapsed_cycles << " cycles passed, equivalent to " << elapsed_cycles * this->seconds_per_cycle * 1000 << " ms." << endl;

    return out_pixels;
};