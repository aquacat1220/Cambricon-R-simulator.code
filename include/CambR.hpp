#ifndef CAMB_R
#define CAMB_R

#include <vector>
#include <unordered_map>

#include "CambRCommon.hpp"
#include "SamplingUnit.hpp"
#include "EncodingUnit.hpp"
#include "MLPUnit.hpp"

#define BEF_SAM 0
#define SAM_IN_PROG 1
#define BEF_ENC 2
#define ENC_IN_PROG 3
#define READY 4

struct CambRStats {
    unsigned int total_cycles;
    unsigned int idle_cycles;
    SamplingUnitStats sampling_unit_stats;
    EncodingUnitStats encoding_unit_stats;
    vector<MlpUnitStats> mlp_unit_statss;
};

/**
 * @brief Class representing a cambricon-r accelerator.
 * 
 */
class CambR {
    public:
    CambR(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z, 
            int min_grid_resolution, int max_grid_resolution,
            float* hash_table,
			vector<vector<float>>& w1_d, vector<vector<float>>& w2_d, vector<vector<float>>& w1_c, vector<vector<float>>& w2_c, vector<vector<float>>& w3_c);

    // Inputs
    /**
     * @brief A vector of input rays.
     * Cleared and (may be) refilled for every call to Cycle().
     * May be empty, if no input is provided for this cycle.
     * 
     */
    vector<Ray> in_rays;
    // Outputs
    /**
     * @brief A Vector of final pixels
     * Host should consume it after every call to Cycle()
    */
    vector<Pixel> out_pixels;

    /**
     * @brief Consume 1 cycle of Cambricon-R accelerator
     * This function should be called every cycle.
    */
    void Cycle();

    private:
    /**
     * @brief A sampling unit
    */
    SamplingUnit sam_unit_;

    /**
     * @brief A encoding unit
    */
    EncodingUnit enc_unit_;

    /**
     * @brief A Vector of 128 MLPunits
    */
    vector<MlpUnit> mlp_units_;

    // For Global buffer
    private:
    /**
     * @brief A buffer to hold rays requested by the host
     * Hashmap from ray idx to ray
    */
    unordered_map<unsigned int, Ray> rays_;

    /**
     * @brief A buffer to hold rays for setting mlp_unit's phi & theta
     * Hashmap from ray idx to ray
    */
    unordered_map<unsigned int, Ray> rays_for_view_;

    /**
     * @brief count value to initially set phi & theta for each mlp_unit
    */
    int initial_view_set_;

    /**
     * @brief A buffer to hold samples, resides in global buffer
     * Hashmap from ray idx to vector of 8 batches of samples
    */
    unordered_map<unsigned int, vector<vector<Sample>>> samples_;

    /**
     * @brief A buffer to hold the next batch of features for the MLP units
     * a Vector of batch of features
     * length 128, one for each MLP unit
    */
    vector<vector<Feature>> features_;

    /**
     * @brief A buffer to hold the current state of the features_ batch for the MLP units
     * a Vector of state enums (BEF_SAM, SAM_IN_PROG, BEF_ENC, ENC_IN_PROG, READY)
     * length 128, one for each MLP unit
    */
    vector<unsigned char> states_;

    private:
    /**
     * @brief Clears all inputs.
     * 
     */
    void ClearInputs();
    /**
     * @brief Clears all outputs.
     * 
     */
    void ClearOutputs();

    private:
    // Statistics.
    bool was_idle_ = true;
    unsigned int total_cycles_ = 0;
    unsigned int idle_cycles_ = 0;

    public:
    // Statistics.
    /**
     * @brief Returns true is the last call to Cycle() was idle.
     * 
     * @return true 
     * @return false 
     */
    bool WasIdle();

    CambRStats GetStats();
};

#endif