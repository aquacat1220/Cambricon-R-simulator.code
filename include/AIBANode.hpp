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
    vector<Request> local_request_buffer_;
    vector<PSum> psum_buffer_;
    vector<HashEntry> sram_bank_;
    vector<Packet> arrival_buffer_;
    // Coordinate in NoC of AIBA nodes. Range between 0 ~ 255.
    unsigned char coord_;

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
     * @brief Routing all Packets in the input buffer.
     * 
     * @param pkt_buffer contains the Packets to be routed
     */
    void Routing(vector<Packet>& pkt_buffer);
    /**
     * @brief Routing a single Packet.
     * 
     * @param pkt is the Packet to be routed.
     */
    void Routing_pkt(Packet pkt);

    public:
    /**
     * @brief Get a constant reference to sram_bank_.
     * 
     * @return constant reference to this->sram_bank_.
     * 
     * @warning Should only be used for testing purposes.
     */
    const vector<HashEntry>& GetSramBank();
    /**
     * @brief Get a constant reference to psum_buffer_.
     * 
     * @return constant reference to this->psum_buffer_.
     * 
     * @warning Should only be used for testing purposes.
     */
    const vector<PSum>& GetPSumBuffer(); 
};

#endif