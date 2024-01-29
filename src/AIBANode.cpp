#include "AIBANode.hpp"

AIBANode::AIBANode(unsigned char coord) {
    this->coord = coord;
}

void AIBANode::Cycle(void) {
    // Clear all outputs before generatign new ones.
    this->ClearOutputs();

    //Requests
    //addr comparison -> LRB or NRB
    while(!in_left_reqs.empty()){
        if(in_left_reqs[0].addr)
    }
        
        //case1. LRB
            //

        //case2. NRB 

    //Packets
    //Routing logic

        //case1. transfer Packet to target node
    vector<Packet> arrival_buffer;
        //case2. destination node
            //Psum computing
            //Psum counter check (==7)

    //Sum
    //transfer to bottom node


    return;
}

void AIBANode::HashTableLoad(HashEntry* hash_table) {
    sram_bank.assign(hash_table, hash_table + (1 << 11));
    return;
}

void AIBANode::ClearOutputs() {
    out_right_reqs.clear();
    out_up_pkts.clear();
    out_down_pkts.clear();
    out_left_pkts.clear();
    out_right_pkts.clear();
    out_down_sums.clear();
}