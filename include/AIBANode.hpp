#ifndef AIBA_NODE
#define AIBA_NODE

#include <vector>

#include "AIBACommon.hpp"

using namespace std;
/**
 * @brief Class representing a single AIBA node.
 * 
 */
class AIBANode {
    public:
    AIBANode(unsigned char coord);

    public:
    // Inputs
    vector<Request> in_left_reqs;
    vector<Packet> in_up_pkts, in_down_pkts, in_left_pkts, in_right_pkts;
    vector<Sum> in_up_sums;
    // Outputs
    vector<Request> out_right_reqs;
    vector<Packet> out_up_pkts, out_down_pkts, out_left_pkts, out_right_pkts;
    vector<Sum> out_down_sums;

    public:
    /**
     * @brief Propagates input values through the node, and write to output values.
     * This function should be called every cycle.
     * Cycle() doesn't magically propagate the outputs to adjacent node's inputs. This is a responsibility of AIBA.
     * 
     */
    void Cycle();

    /**
     * @brief Loads the hash table into sram_bank.
     * 
     * @param hash_table Pointer to start of hash table as an array of HashEntry.
     * Array should be of length 2**11 (2**13 bytes).
     */
    void HashTableLoad(HashEntry* hash_table);

    private:
    vector<Request> local_request_buffer;
    vector<PSum> psum_buffer;
    vector<HashEntry> sram_bank;
    // Coordinate in NoC of AIBA nodes. Range between 0 ~ 255.
    unsigned char coord;

    private:
    /**
     * @brief Clears all outputs.
     * 
     */
    void ClearOutputs();
};

#endif