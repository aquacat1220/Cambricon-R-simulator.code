#include "AIBANode.hpp"

AIBANode::AIBANode(unsigned char coord) {
    this->coord = coord;
}

void AIBANode::Cycle(void) {
    //Requests
    //addr comparison -> LRB or NRB
    //while(in_left_reqs.)
    //if(in_left_reqs[0].addr)
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

void AIBANode::HashTableLoad(int* hash_table) {
    sram_bank.assign(hash_table, hash_table + (1 << 11));
    return;
}