#ifndef AIBA_ARC
#define AIBA_ARC

#include <vector>
#include <unordered_map>

#include "AIBAPP.hpp"
#include "AIBANode.hpp"

class AIBA {
    public:
    AIBA(int grid_resolution);

    public:
    // Inputs
    /**
     * @brief 32 points to process as a single batch.
     * Cleared and refilled for every call to Cycle().
     * 
     */
    vector<Point> in_points;
    // Outputs
    /**
     * @brief Vector of calculated sums from the same batch.
     * AIBA processes multiple batches at the same time, and it is possible that more than two batches finish calculating at the same cycle.
     * Each element in out_sums is a vector of 32 calculated sums coming from the same batch.
     * 
     */
    vector<vector<Sum>> out_sums;
    
    public:
    /**
     * @brief Propagates input values through the AIBA unit, and writes to output values.
     * This function should be called every cycle.
     * Cycle() doesn't magically propagate the outputs to MLP unit or the global buffer. This is a responsibility of the system.
     * 
     */
    void Cycle(void);

    private:
    /**
     * @brief The grid resolution this AIBA unit is responsible of.
     * Used in preprocessor, should be passed in during construction.
     * 
     */
    int grid_resolution_;
    /**
     * @brief The preprocessing unit of this AIBA.
     * 
     */
    AIBAPP AIBAPP_;
    /**
     * @brief 256 AIBA nodes arranged as 16x16 mesh.
     * 
     */
    vector<AIBANode> AIBANodes_;
    /**
     * @brief Internal buffer which maps RIdx to their output sums.
     * When 32 sums accumulate, AIBA will pop those sums into a vector, and return it in out_sums.
     * 
     */
    unordered_multimap<unsigned char, Sum> sum_buffer_;

    private:
    /**
     * @brief Propagates output values of components (AIBAPP_, AIBANodes_) to their inputs.
     * This function should only be called in Cycle(), after ticking all of the components.
     * 
     */
    void Transition(void);
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
     * @brief Propagates output values from a node to their adjacent components.
     * This function is called, and should only be called in Transition.
     * 
     * @param coord Coordinate of the aiba node to process.
     * 
     */
    void NodeTransition(unsigned char coord);
    /**
     * @brief Checks if the sum_buffer_ has all 32 sums ready for the given ridx, and inserts it into out_sums if so.
     * 
     * @param ridx The request index to check for.
     */
    void CheckSumBuffer(unsigned char ridx);
};

#endif