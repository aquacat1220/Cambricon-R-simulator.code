#include "AIBANode.hpp"

AIBANode::AIBANode(unsigned char coord) {
    this->coord_ = coord;
}

void AIBANode::Cycle(void) {
    // Clear all outputs before generating new ones.
    this->ClearOutputs();

    // Whether the vertex address belongs to the current node's sram bank.
    for(vector<Request>::iterator itr = in_left_reqs.begin(); itr!=in_left_reqs.end(); ++itr){
        if(coord_ == static_cast<unsigned char>((itr->addr >> 11) & 0xFF)){
            // Store the Request in LRB 
            local_request_buffer_.push_back(*itr);
        }else{
            // Transfer the Request to right node
            out_right_reqs.push_back(*itr);
        }
    }

    if(!local_request_buffer_.empty()) {
        // SRAM bank access
        Request req = *local_request_buffer_.begin();
        local_request_buffer_.erase(local_request_buffer_.begin());
        HashEntry hashentry = sram_bank_[req.addr & 0x000007FF];

        // Apply weight to hash entry
        hashentry.elem0 *= req.weight;
        hashentry.elem1 *= req.weight;
        
        // Generating Packet with encoded data
        Packet sram_accessed_pkt;
        sram_accessed_pkt.ridx = req.ridx;
        sram_accessed_pkt.bidx = req.bidx;
        sram_accessed_pkt.pidx = req.pidx;
        sram_accessed_pkt.data = hashentry;
        sram_accessed_pkt.dest = req.dest;

        //Routing
        this->Routing_pkt(sram_accessed_pkt);
    }
    
    //Packets
    //Routing logic
    this->Routing(in_up_pkts);
    this->Routing(in_down_pkts);
    this->Routing(in_left_pkts);
    this->Routing(in_right_pkts);

    // Transfer input sums from up node to down node.
    out_down_sums.insert(out_down_sums.end(), in_up_sums.begin(), in_up_sums.end());

    // Iterate over arrive packets and accumulate into psum_buffer.
    for (const Packet &pkt : arrival_buffer_) {
        unsigned int first = 0;
        // First check for preexisting matching psums.
        for (PSum &psum : psum_buffer_) {
            if ((pkt.ridx == psum.ridx) && (pkt.bidx == psum.bidx) && (pkt.pidx == psum.pidx)) {
                first = 1;
                // If matching psum exists, accumulate packet.
                psum.psum.elem0 += pkt.data.elem0;
                psum.psum.elem1 += pkt.data.elem1;
                psum.cnt++;

                // And if psum is full, convert it to a Sum and send it out.
                if (psum.cnt == 8) {
                    out_down_sums.emplace_back(psum.ridx, psum.bidx, psum.pidx, psum.psum);
                }

                break;
            }
        }
        if (first == 0) {
            // If there are no matching psums, add a new one to the buffer!
            psum_buffer_.emplace_back(pkt.ridx, pkt.bidx, pkt.pidx, pkt.data, 1);
        }
    }
    arrival_buffer_.clear();

    this->ClearInputs();

    return;
}

void AIBANode::HashTableLoad(HashEntry* hash_table) {
    sram_bank_.assign(hash_table, hash_table + (1 << 11));
    return;
}

void AIBANode::ClearInputs() {
    in_left_reqs.clear();
    in_up_pkts.clear();
    in_down_pkts.clear();
    in_left_pkts.clear();
    in_right_pkts.clear();
    in_up_sums.clear();
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
    unsigned char my_row = this->coord_ >> 4;
    unsigned char my_col = this->coord_ & 0xF;
    unsigned char target_row = pkt.dest >> 4;
    unsigned char target_col = pkt.dest & 0xF;

    if (target_row < my_row)
        out_up_pkts.push_back(pkt);
    else if (target_row > my_row)
        out_down_pkts.push_back(pkt);
    else {
        if (target_col < my_col)
            out_left_pkts.push_back(pkt);
        else if (target_col > my_col)
            out_right_pkts.push_back(pkt);
        else
            arrival_buffer_.push_back(pkt);
    }

}

const vector<HashEntry>& AIBANode::GetSramBank() {
    const auto& ref = this->sram_bank_;
    return ref;
}

const vector<PSum>& AIBANode::GetPSumBuffer() {
    const auto& ref = this->psum_buffer_;
    return ref;
}