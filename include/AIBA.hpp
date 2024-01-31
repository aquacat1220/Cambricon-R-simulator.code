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
     * @brief Output buffer which maps RIdx to their output 32 sums.
     * AIBA will only fill out_req_buffer. It is your responsibility to check for finished requests and consume it.
     * Unlike other output variables, out_sum_buffer should not be invalidated by external classes, unless the request is fully finished and broadcasted.
     * 
     */
    unordered_map<unsigned char, vector<Sum>> out_sum_buffer;
    
    public:
    /**
     * @brief Propagates input values through the AIBA unit, and writes to output values.
     * This function should be called every cycle.
     * Cycle() doesn't magically propagate the outputs to MLP unit or the global buffer. This is a responsibility of the system.
     * 
     */
    void Cycle(void);
    void Transition(void);

    private:
    int grid_resolution_;
    AIBAPP AIBAPP_;
    vector<AIBANode> AIBANodes_;
};

#endif