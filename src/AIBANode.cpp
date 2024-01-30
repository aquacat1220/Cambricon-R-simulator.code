#include "AIBANode.hpp"

AIBANode::AIBANode(unsigned char coord) {
    this->coord = coord;
}

void AIBANode::Cycle(void) {
    // Clear all outputs before generating new ones.
    this->ClearOutputs();

    // Whether the vertex address belongs to the current node's sram bank.
    for(vector<Request>::iterator itr = in_left_reqs.begin(); itr!=in_left_reqs.end(); ++itr){
        if(coord == static_cast<unsigned char>((itr->addr >> 11) & 0xFF)){
            // Store the Request in LRB 
            local_request_buffer.push_back(*itr);
        }else{
            // Transfer the Request to right node
            out_right_reqs.push_back(*itr);
        }
    }

    // SRAM bank access
    Request req = *local_request_buffer.begin();
    local_request_buffer.erase(local_request_buffer.begin());
    HashEntry hashentry = sram_bank[req.addr & 0x000007FF];

    // Apply weight to hash entry
    hashentry.elem0 *= req.weight;
    hashentry.elem1 *= req.weight;
    
    // Generating Packet with encoded data
    Packet sram_accessed_pkt;
    sram_accessed_pkt.ridx = req.ridx;
    sram_accessed_pkt.pidx = req.pidx;
    sram_accessed_pkt.data = hashentry;
    sram_accessed_pkt.dest = req.dest;

    //Packets
    //Routing logic
    this->Routing_pkt(sram_accessed_pkt);
    this->Routing(in_up_pkts);
    this->Routing(in_down_pkts);
    this->Routing(in_left_pkts);
    this->Routing(in_right_pkts);
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

void AIBANode::Routing(vector<Packet> &pkt_buffer){
    for(vector<Packet>::iterator itr = pkt_buffer.begin(); itr!=pkt_buffer.end(); ++itr){
        Routing_pkt(*itr);
    }
}

void AIBANode::Routing_pkt(Packet pkt){
    unsigned char k = coord - pkt.dest;
    unsigned char l = coord/16 - pkt.dest/16;
    if (k==0){
        arrival_buffer.push_back(pkt);
    }else if (k<0){
        if (l==0) {
            out_right_pkts.push_back(pkt);
        }else{
            out_down_pkts.push_back(pkt);
        }
    }else{
        if (l==0) {
            out_left_pkts.push_back(pkt);
        }else{
            out_up_pkts.push_back(pkt);
        }
    }

}