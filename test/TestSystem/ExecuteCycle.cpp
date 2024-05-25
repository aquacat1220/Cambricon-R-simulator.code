#include "System.hpp"

#include <cassert>
#include <cmath>
#include <random>
#include <iostream>

vector<SystemRay> create_sysrays(mt19937 rand_engine, uniform_real_distribution<>& real_dist) {
    vector<SystemRay> rays;
    rays.reserve(256);

    for (unsigned int i = 0; i < 256; ++i) {
        rays.push_back(SystemRay{
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

int main(){
    vector<float> hash_table(1 << 24, 1.0f);

    vector<vector<float>> w1_density(32, vector<float>(64, 1e-4));
    vector<vector<float>> w2_density(64, vector<float>(16, 1e-4));
    vector<vector<float>> w1_color(32, vector<float>(64, 1e-4));
    vector<vector<float>> w2_color(64, vector<float>(64, 1e-4));
    vector<vector<float>> w3_color(64, vector<float>(3, 1e-4));

    System system = System(0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 1<<4 ,1<<12, &hash_table.front(), w1_density, w2_density, w1_color, w2_color, w3_color);

    mt19937 rand_engine(20240515);
    uniform_real_distribution<> real_dist(0.0, M_PI_2);

    system.request(create_sysrays(rand_engine, real_dist));

    vector<SystemPixel> output = system.handle_requests();

    cout << output.size() << endl;

    assert(output.size() == 256);

    return 0;
}