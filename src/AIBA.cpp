#include "AIBA.hpp"

#include <iostream>

AIBA::AIBA(int grid_resolution) : AIBAPP_(grid_resolution) {
    // Initialize AIBANode mesh.
    this->AIBANodes_.reserve(16 * 16);
    for (unsigned int coord = 0; coord < 256; coord++) {
        AIBANodes_.emplace_back(static_cast<unsigned char>(coord));
    }

    if (AIBANodes_.size() != 256)
        cout << ("[ERROR] AIBA::AIBANode_.size() is not 256.") << endl;
}

void AIBA::Cycle(){
    // Preprocessing by AIBAPP
    AIBAPP_.in_point_batch = in_points;
    AIBAPP_.Cycle();

    // 16x16 AIBANode mesh Cycle()
    for (AIBANode &aibanode : AIBANodes_) {
        aibanode.Cycle();
    }

    this->Transition();
}

void AIBA::Transition(){
    //address crossbar
    for(Request &req : AIBAPP_.out_reqs) {
        unsigned char target_row = static_cast<unsigned char> ((req.addr >> 15) & 0x0F);
        AIBANodes_[target_row * 16].in_left_reqs.push_back(req);
    }
    //output crossbar
    
}