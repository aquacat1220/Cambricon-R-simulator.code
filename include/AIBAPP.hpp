#ifndef AIBA_PP
#define AIBA_PP

#include <vector>

#include "AIBACommon.hpp"

using namespace std;

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
     * The size of the vector MUST be 32.
     * Should be correctly set before the call to Cycle().
     * 
     */
    vector<Point> in_point_batch;
    // Outputs
    /**
     * @brief 256 Requests for the address crossbar to dispatch to AIBA nodes.
     * As long as the input size is 32, the output size is guaranteed to be 256.
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

    /**
     * @brief Clears all outputs.
     * 
     */
    void ClearOutputs();

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
    /**
     * @brief The Request::ridx value used for this batch.
     * Each call to Cycle() will increment this value by 1.
     * @warning There are currently no safety mechanisms for overflow.
     * Though this is very unlikely to happen, if the AIBA is currently handling more than 256 requests,
     * two independent batches may share the same request index.
     * @todo Implement a safety mechanism somewhere.
     * 
     */
    unsigned char ridx_reg_;

    private:
    /**
     * @brief Hash function to access hash table.
     * 
     * @param gridx x coordinate
     * @param gridy y coordinate.
     * @param gridz z coordinate.
     * @return Calculated hash value.
     * 
     * @warning InstantNGP uses bitwise XOR, Cambricon-R uses addition. Current implementation uses XOR (original approach).
     */
    static unsigned int Hash(int gridx, int gridy, int gridz);
};

#endif