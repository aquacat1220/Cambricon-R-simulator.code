#ifndef SYSTEM
#define SYSTEM

#include <vector>

#include "CambR.hpp"

/**
 * @brief Struct representing a single ray to render the image, but without ray index.
 * 
 */
struct SystemRay {
    float x;
    float y;
    float z;
    float theta;
    float phi;
};

/**
 * @brief Struct representing a single pixel in an image, but without ray index.
 * 
 */
struct SystemPixel {
    float r;
    float g;
    float b;
};

class System {
    private:
    // Configs
    float seconds_per_cycle = 1 / (750 * 1000000);
    unsigned int cycle_count_ = 0;

    public:
    System(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z, 
            int min_grid_resolution, int max_grid_resolution,
            float* hash_table,
			vector<vector<float>>& w1_d, vector<vector<float>>& w2_d, vector<vector<float>>& w1_c, vector<vector<float>>& w2_c, vector<vector<float>>& w3_c);

    private:
    CambR camb_r_;

    unsigned int next_ridx_ = 0;
    unsigned int last_ridx_ = -1;

    public:
    /**
     * @brief Queries the simulator to calculate pixel colors for `rays`.
     * 
     * @param rays A vector of `SystemRay`s to query.
     */
    void request(vector<SystemRay> rays);
    
    /**
     * @brief Runs the simulator until all previously queried requests are completed.
     * 
     * @return vector<SystemPixel> A vector of calculated pixel colors, in the order of the queries.
     */
    vector<SystemPixel> handle_requests();
};

#endif