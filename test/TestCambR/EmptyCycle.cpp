#include "CambR.hpp"

#include <cassert>
#include <iostream>

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
    CambRStats init_stat = cambr.GetStats();
    assert(init_stat.total_cycles == 0);
    assert(init_stat.idle_cycles == 0);
    assert(init_stat.sampling_unit_stats.total_cycles == 0);
    assert(init_stat.sampling_unit_stats.idle_cycles == 0);
    assert(init_stat.encoding_unit_stats.total_cycles == 0);
    assert(init_stat.encoding_unit_stats.idle_cycles == 0);
    assert(init_stat.encoding_unit_stats.aiba_statss[0].total_cycles == 0);
    assert(init_stat.encoding_unit_stats.aiba_statss[0].idle_cycles == 0);
    for (auto mlp_unit_stats: init_stat.mlp_unit_statss) {
        assert(mlp_unit_stats.total_cycles == 0);
        assert(mlp_unit_stats.idle_cycles == 0);
    }

    for (int i = 0; i < 32; i++) {
        cambr.Cycle();
    }
    CambRStats final_stat = cambr.GetStats();
    assert(final_stat.total_cycles == 32);
    assert(final_stat.idle_cycles == 32);
    assert(final_stat.sampling_unit_stats.total_cycles == 32);
    assert(final_stat.sampling_unit_stats.idle_cycles == 32);
    assert(final_stat.encoding_unit_stats.total_cycles == 32);
    assert(final_stat.encoding_unit_stats.idle_cycles == 32);
    assert(final_stat.encoding_unit_stats.aiba_statss[0].total_cycles == 32);
    assert(final_stat.encoding_unit_stats.aiba_statss[0].idle_cycles == 32);
    for (auto mlp_unit_stats: final_stat.mlp_unit_statss) {
        assert(mlp_unit_stats.total_cycles == 32);
        assert(mlp_unit_stats.idle_cycles == 32);
    }
}