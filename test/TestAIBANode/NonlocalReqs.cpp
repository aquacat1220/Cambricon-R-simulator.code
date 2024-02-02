#include "AIBANode.hpp"

#include <algorithm>

int main() {
    // Create AIBANode with coordinate (0, 0).
    AIBANode node = AIBANode(0);

    // Load it with zeroed hashtable
    vector<HashEntry> orig_hash_table;
    orig_hash_table.assign((1 << 11),{ 1.0f, 1.0f });
    node.HashTableLoad(&orig_hash_table.front());

    // Create vector of non-local requests.
    vector<Request> nonlocal_reqs = {
        Request {
            .ridx = 0,
            .pidx = 0,
            .addr = 1 << 11,
            .weight = 0.5,
            .dest = 255
        },
        Request {
            .ridx = 10,
            .pidx = 10,
            .addr = 1 << 11,
            .weight = 0.8,
            .dest = 0
        },
        Request {
            .ridx = 20,
            .pidx = 20,
            .addr = 1 << 11,
            .weight = 0.2,
            .dest = 1
        },
        Request {
            .ridx = 30,
            .pidx = 30,
            .addr = 15 << 11,
            .weight = 0.5,
            .dest = 255
        }
    };

    

    node.in_left_reqs = nonlocal_reqs;
    node.Cycle();
    auto &out_reqs = node.out_right_reqs;

    sort(nonlocal_reqs.begin(), nonlocal_reqs.end());
    sort(out_reqs.begin(), out_reqs.end());

    if (nonlocal_reqs != out_reqs) {
        return 1;
    }

    return 0;
}