#ifndef AIBA_NODE
#define AIBA_NODE

#include <vector>

#include "AIBACommon.hpp"

using namespace std;

class AIBANode {
    public:
    AIBANode();

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
    void Cycle();
    void HashTableLoad(int* hash_table);

    private:
    vector<Request> local_request_buffer;
    vector<PSum> psum_buffer;
    vector<int> sram_bank;
    // Coordinate in NoC of AIBA nodes. Range between 0 ~ 255.
    unsigned char coord;
};

#endif