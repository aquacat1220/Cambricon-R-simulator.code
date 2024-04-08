#ifndef MLP_UNIT
#define MLP_UNIT

#include <vector>

#include "CambRCommon.hpp"

using namespace std;

class MlpUnit {
    public:
    MlpUnit(unsigned int ridx_);

    public:
    // Inputs
    /**
     * @brief Vector of 32 encoded features from the same batch
     * Cleared and refilled when remain_cycle = 0
    */
    vector<Feature> in_features;

    // Outputs
    /**
     * @brief Final pixel color of the corresponding ray
     * 
    */
    Pixel out_pixel;

    /**
    * @brief ridx of the ray processed by this MLP Unit
    */
    unsigned int ridx;

    /**
    * @brief View direction Theta & Phi
    * Projected onto the first 16 coefficients of the spherical harmonics basis
    * Cleared and refilled when computed ray is changed.
    */
    float theta;
    float phi;

    /**
    * @brief Dendity threshold value for early termination
    * Suppose that this value is fixed value stored in ROM. 
    */
    float threshold;

    public:
    /**
    * @brief Compute density and color throuh density network and color network
    * This function should be called every cycle.
    * Computes a single batch untill remain_cycle_=0 .
    * Cycle() doesn't magically propagate the outputs to the global buffer. This is a responsibility of the system.
    */
    void Cycle();
    /**
    * @brief Laods the MLP weights to MLP unit
    * Since we consider only inference, this function called only one time initially by the system  
    * MLP weight consists of several matrices (32x64, 64x16 for density network & 32x64, 64x64, 64x3 for color network)
    */
    void MlpWeightLoad(vector<vector<float>>& w1_d, vector<vector<float>>& w2_d, vector<vector<float>>& w1_c, vector<vector<float>>& w2_c, vector<vector<float>>& w3_c);

    private:
    /**
    * @brief Remained cycles for a single batch MLP computation
    * If this variable != 0, MLP unit cannot proceed another batch
    */
    int remain_cycle_; 

    /**
    * @brief MLP weights for MLP
    * (32x64, 64x16 for density network & 32x64, 64x64, 64x3 for color network)
    */
    vector<vector<float>> w1_density_;
    vector<vector<float>> w2_density_;
    vector<vector<float>> w1_color_;
    vector<vector<float>> w2_color_;
    vector<vector<float>> w3_color_;

    /**
    * @brief Save the density and color values of the sample points of the corresponding ray.
    * 
    */
    vector<float> densities_;
    vector<vector<float>> colors_;
    vector<float> distances_;

    /**
    * @brief For checking whether density over threshold or not
    */
    float accumulate_dens_;

    /**
     * @brief true if MLP unit has output
    */
    bool has_output;

    private:
    /**
    * @brief Clears all inputs
    * 
    */
    void ClearInputs();

    /**
    * @brief Clears private variables
    * 
    */
    void ClearLocals();

    /**
    * @brief Compute final color. (Formula in NeRF paper)
    * 
    */
    Pixel ComputeColor();

    public:
    /**
     * @brief Return true if the MLP unit has output pixel
     * System uses this funciton for checking whether the MLP unit has output or not.
    */
    bool GetHasOutput();

    /**
    * @brief Returns true if MLP finished calculation and waiting for input
    * System uses this function for checking corresponding MLP Unit done batch computation.
    */
    bool is_idle();

    /**
    * @brief Set distances_ to meaningless distance values b/w two samples for testing.
    * Distance value is required for computing color.
    * However, we don't have coordinate value in Feature structure.
    * 
    * @warning Should only be used for testing purposes.
    */
    void SetDistance();
};

#endif