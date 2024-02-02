#include "AIBANode.hpp"

#include <algorithm>
#include <iostream>
#include <cassert>

int main() {
    // Create AIBANode with coordinate (1, 1).
    AIBANode node = AIBANode(17);

    // Load it with zeroed hashtable
    vector<HashEntry> orig_hash_table;
    orig_hash_table.assign((1 << 11),{ 1.0f, 1.0f });
    orig_hash_table[000] = HashEntry { .elem0 = 0000.0f, .elem1 = 0000.0f };
    orig_hash_table[100] = HashEntry { .elem0 = 1000.0f, .elem1 = 1000.0f };
    orig_hash_table[200] = HashEntry { .elem0 = 2000.0f, .elem1 = 2000.0f };
    orig_hash_table[300] = HashEntry { .elem0 = 3000.0f, .elem1 = 3000.0f };
    orig_hash_table[400] = HashEntry { .elem0 = 4000.0f, .elem1 = 4000.0f };
    node.HashTableLoad(&orig_hash_table.front());

    // Create vector of local requests.
    vector<Request> local_reqs = {
        // Will result in local packet.
        Request {
            .ridx = 0,
            .pidx = 0,
            .addr = (17 << 11) + 000,
            .weight = 0.5,
            .dest = 17
        },
        // Up packet.
        Request {
            .ridx = 10,
            .pidx = 10,
            .addr = (17 << 11) + 100,
            .weight = 0.8,
            .dest = 1 // (0,1)
        },
        // Down packet.
        Request {
            .ridx = 20,
            .pidx = 20,
            .addr = (17 << 11) + 200,
            .weight = 0.2,
            .dest = 33 // (2,1)
        },
        // Left packet.
        Request {
            .ridx = 30,
            .pidx = 30,
            .addr = (17 << 11) + 300,
            .weight = 0.5,
            .dest = 16 // (1,0)
        },
        // Right packet.
        Request {
            .ridx = 40,
            .pidx = 40,
            .addr = (17 << 11) + 400,
            .weight = 0.5,
            .dest = 18 // (1,2)
        }
    };

    node.in_left_reqs = local_reqs;

    vector<Request> acc_out_right_reqs;
    vector<Packet> acc_out_up_pkts;
    vector<Packet> acc_out_down_pkts;
    vector<Packet> acc_out_left_pkts;
    vector<Packet> acc_out_right_pkts;

    for (int i = 0; i < 50; i++) {
        node.Cycle();
        acc_out_right_reqs.insert(acc_out_right_reqs.end(), node.out_right_reqs.begin(), node.out_right_reqs.end());
        acc_out_up_pkts.insert(acc_out_up_pkts.end(), node.out_up_pkts.begin(), node.out_up_pkts.end());
        acc_out_down_pkts.insert(acc_out_down_pkts.end(), node.out_down_pkts.begin(), node.out_down_pkts.end());
        acc_out_left_pkts.insert(acc_out_left_pkts.end(), node.out_left_pkts.begin(), node.out_left_pkts.end());
        acc_out_right_pkts.insert(acc_out_right_pkts.end(), node.out_right_pkts.begin(), node.out_right_pkts.end());
    }

    sort(acc_out_right_reqs.begin(), acc_out_right_reqs.end());
    sort(acc_out_up_pkts.begin(), acc_out_up_pkts.end());
    sort(acc_out_down_pkts.begin(), acc_out_down_pkts.end());
    sort(acc_out_left_pkts.begin(), acc_out_left_pkts.end());
    sort(acc_out_right_pkts.begin(), acc_out_right_pkts.end());

    assert(acc_out_right_reqs == vector<Request>({}));
    assert(acc_out_up_pkts == vector<Packet>({
        Packet {
            .ridx = 10,
            .pidx = 10,
            .data = { 800.0, 800.0 },
            .dest = 1
        }
    }));
    assert(acc_out_down_pkts == vector<Packet>({
        Packet {
            .ridx = 20,
            .pidx = 20,
            .data = { 400.0, 400.0 },
            .dest = 33
        }
    }));
    assert(acc_out_left_pkts == vector<Packet>({
        Packet {
            .ridx = 30,
            .pidx = 30,
            .data = { 1500.0, 1500.0 },
            .dest = 16
        }
    }));
    assert(acc_out_right_pkts == vector<Packet>({
        Packet {
            .ridx = 40,
            .pidx = 40,
            .data = { 2000.0, 2000.0 },
            .dest = 18
        }
    }));

    return 0;
}