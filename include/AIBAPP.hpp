#ifndef AIBA_PP
#define AIBA_PP

#include <vector>

#include "AIBACommon.hpp"

using namespace std;

struct AIBAPPStats {
    unsigned int total_cycles;
    unsigned int idle_cycles;
};

/**
 * @brief Class representing an AIBA preprocessing unit.
 * AIBAPP takes 32 point input batches and produce 256 requests for AIBANode.
 * 
 */
class AIBAPP {
    public:
    AIBAPP(int grid_resolution);

    public:
    // Inputs
    /**
     * @brief 32 Points to process as a single batch.
     * The size of the vector MUST be 32 or 0, and if it is 0, AIBAPP will have an empty cycle.
     * Should be correctly set before the call to Cycle().
     * 
     */
    vector<Point> in_point_batch;
    // Outputs
    /**
     * @brief 256 Requests for the address crossbar to dispatch to AIBA nodes.
     * If input size was 32, output size is 256. If input was empty, output is empty too.
     * Cleared and refilled for every call to Cycle().
     * 
     */
    vector<Request> out_reqs;

    public:
    /**
     * @brief Propagates input values through the preprocessor, and write to output values.
     * This function should be called every cycle.
     * Cycle() doesn't magically propagate the outputs to AIBA nodes' inputs. This is a responsibility of AIBA.
     * 
     */
    void Cycle();

    private:
    /**
     * @brief The grid resolution this preprocessor unit is responsible of.
     * Used during grid point calculation.
     * Should be passed in from its AIBA.
     * 
     */
    int grid_resolution_;
    /**
     * @brief The destination AIBA node coordinate used for this batch.
     * Each call to Cycle() will increment this value by 32.
     * 
     */
    unsigned char dest_reg_;

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
    /**
     * @brief Hash function to access hash table.
     * 
     * @param grid_x x coordinate.
     * @param grid_y y coordinate.
     * @param grid_z z coordinate.
     * @return Calculated hash value.
     * 
     * @warning InstantNGP uses bitwise XOR, Cambricon-R uses addition. Current implementation uses XOR (original approach).
     */
    static unsigned int Hash(int grid_x, int grid_y, int grid_z);

    /**
     * @brief Calculates weights for trilinear interpolation.
     * 
     * @param offx Offset of target x coordinate scaled to 0 ~ 1.
     * @param offy Offset of target y coordinate scaled to 0 ~ 1.
     * @param offz Offset of target z coordinate scaled to 0 ~ 1.
     * @return Size 8 vector of calculated weights, in the order of 000, 001, 010, 011, 100, 101, 110, 111.
     */
    static vector<float> TriLerp(float offx, float offy, float offz);

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
    AIBAPPStats GetStats();
};

#endif