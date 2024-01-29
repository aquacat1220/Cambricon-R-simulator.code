#include "AIBANode.hpp"

AIBANode::AIBANode(unsigned char coord) {
    this->coord = coord;
}

void AIBANode::Cycle() {
    return;
}

void AIBANode::HashTableLoad(int* hash_table) {
    sram_bank.assign(hash_table, hash_table + (1 << 11));
    return;
}