#ifndef SAMPLING_UNIT
#define SAMPLING_UNIT

#include <vector>
#include <cassert>

#include "CambRCommon.hpp"

using namespace std;

/**
 * @brief Class representing a sampling unit.
 * Produces samples in batches for each ray, for them to be encoded by the encoding unit.
 * 
 */
class SamplingUnit {
    public:
    /**
     * @brief A constructor taking in a bounding box of the scene.
     * 
     * @param min_x 
     * @param max_x 
     * @param min_y 
     * @param max_y 
     * @param min_z 
     * @param max_z 
     */
    SamplingUnit(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z): min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y), min_z(min_z), max_z(max_z) { assert(this->min_x <= this->max_x); assert(this->min_y <= this->max_y); assert(this->min_z <= this->max_z);}

    public:
    // Inputs
    /**
     * @brief A single ray to process this cycle.
     * Cleared on every call to Cycle(). If left empty, the sampling unit will be idle.
     * 
     * @warning While the type is vector<Ray>, this variable should contain 0 or 1 Ray, not multiple ones.
     * This is because I am too lazy to learn about C++ optionals.
     * 
     */
    vector<Ray> in_ray;

    // Outputs
    /**
     * @brief A vector of sample batches.
     * Specifically, the outer vector is of length 8 (8 batches per ray), and the inner vector is of length 32 (32 samples per batch).
     * If the sampling unit was idle during the last Cycle(), might be empty.
     * Overwritten on every call to Cycle(), so make sure to copy or consume it.
     * 
     */
    vector<vector<Sample>> out_sample_batches;

    public:
    /**
     * @brief Propagates input values through the smapling unit, and writes to output values.
     * This function should be called every cycle.
     * Cycle() doesn't magically propagate outputs to the encoding unit. This is a responsibility of the system.
     * 
     */
    void Cycle();

    private:
    /**
     * @brief The bounding box of the scene.
     * 
     */
    float min_x, max_x, min_y, max_y, min_z, max_z;

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
};


#endif