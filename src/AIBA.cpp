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
    //_AIBAPP.Cycle()
    //_AIBANodes

}

void AIBA::Transition(){
    //address crossbar
    //output crossbar

}