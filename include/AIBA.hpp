#ifndef AIBA_ARC
#define AIBA_ARC

#include <vector>
#include <unordered_map>
#include <utility>

#include "AIBACommon.hpp"
#include "AIBAPP.hpp"
#include "AIBANode.hpp"

struct PairHash {
    size_t operator() (const pair<unsigned int, unsigned char>& p) const {
        auto h1 = hash<unsigned int>{}(p.first);
        auto h2 = hash<unsigned char>{}(p.second);
        return h1 ^ h2;
    }
};

struct AIBAStats {
    unsigned int total_cycles;
    unsigned int idle_cycles;
    AIBAPPStats aiba_pp_stats;
    vector<AIBANodeStats> aiba_node_statss;
};

/**
 * @brief Class representing a single AIBA unit.
 * Each AIBA unit is responsible for a single level of multiresolution hash table
 * , and is capable of encoding 32 points per cycle.
 * 
 */
class AIBA {
    public:
    AIBA(int grid_resolution);

    public:
    // Inputs
    /**
     * @brief 32 points to process as a single batch.
     * Cleared and refilled for every call to Cycle().
     * May be empty, if no input is provided for this cycle. In this case, AIBA will only Cycle() its components.
     * 
     */
    vector<Point> in_points;
    // Outputs
    /**
     * @brief Vector of calculated sums from the same batch.
     * AIBA processes multiple batches at the same time, and it is possible that more than two batches finish calculating at the same cycle.
     * Each element in out_sums is a vector of 32 calculated sums coming from the same batch.
     * May be empty, if there were no batches encoded this cycle.
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
    /**
     * @brief Loads the hash table into AIBA nodes.
     * 
     * @param hash_table Pointer to start of hash table as an array of HashEntry.
     * Array should be of length 256 * 2**11.
     */
    void HashTableLoad(HashEntry* hash_table);

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
    unordered_multimap<pair<unsigned int, unsigned char>, Sum, PairHash> sum_buffer_;

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
    void CheckSumBuffer(unsigned int ridx, unsigned char bidx);

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
    AIBAStats GetStats();
};

#endif