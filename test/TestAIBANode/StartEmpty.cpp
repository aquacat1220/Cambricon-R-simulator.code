#include "AIBANode.hpp"

#include <iostream>

int main() {
    // Create AIBANode with coordinate (0, 0).
    AIBANode node = AIBANode(0);

    // Load it with zeroed hashtable
    vector<HashEntry> orig_hash_table;
    orig_hash_table.assign((1 << 11),{ 1.0f, 1.0f });
    node.HashTableLoad(&orig_hash_table.front());

    // All input/output lines should initially be empty.
    if (!(node.in_left_reqs.empty() && node.in_up_pkts.empty() && node.in_down_pkts.empty() && \
    node.in_left_pkts.empty() && node.in_right_pkts.empty() && node.in_up_sums.empty() && \
    node.out_right_reqs.empty() && node.out_up_pkts.empty() && node.out_down_pkts.empty() && \
    node.out_left_pkts.empty() && node.out_right_pkts.empty() && node.out_down_sums.empty())) {
        return 1;
    }

    return 0;
}