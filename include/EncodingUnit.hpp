#ifndef ENCODING_UNIT
#define ENCODING_UNIT

#include <vector>

#include "CambRCommon.hpp"
#include "AIBA.hpp"

using namespace std;

class EncodingUnit {
    public:
    EncodingUnit(int min_grid_resolution, int max_grid_resolution);

    public:
    // Inputs
    /**
     * @brief 32 samples to process as a single batch.
     * Cleared and refilledfor every call to Cycle().
     * 
     */
    vector<Sample> in_samples;

    // Outputs
    /**
     * @brief Vector of encoded features from the same batch.
     * EncodingUnit processes multiple batches at the same time, and it is possible that more than one batch finish encoding at the same cycle.
     * Each element in out_features is a vector of 32 encoded features coming from the same batch.
     * 
     */
    vector<vector<Feature>> out_features;

    public:
    /**
     * @brief Propagates input values through the encoding unit, and writes to output values.
     * This function should be called every cycle.
     * Cycle() doesn't magically propagate the outputs to MLP unit or the global buffer. This is a responsibility of the system.
     * 
     */
    void Cycle();
    /**
     * @brief Loads the hash table into AIBA units.
     * 
     * @param hash_table Pointer to start of hash table as an array of floats.
     * Array should be of length 16(levels) * 256(nodes) * 2**11(entries) * 2(floats per entry) (= 2**24 floats)
     */
    void HashTableLoad(float* hash_table);

    private:
    /**
     * @brief Vector storing grid resolutions of each level. Length will be 16.
     * 
     */
    vector<int> grid_resolutions;
    /**
     * @brief Vector storing AIBA units for each level. Length will be 16.
     * 
     */
    vector<AIBA> aiba_units;
};

#endif