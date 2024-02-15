#include "AIBANode.hpp"

#include <algorithm>
#include <iostream>
#include <cassert>

int main() {
    // Create AIBANode with coordinate (1, 1).
    AIBANode node = AIBANode(17);

    // Load it with zeroed hashtable
    vector<HashEntry> orig_hash_table;
    orig_hash_table.assign((1 << 11), {1.0f, 1.0f});
    for (int i = 0; i < 8; ++i){
        orig_hash_table[100 * i] = HashEntry { .elem0 = 10.0f, .elem1 = 10.0f };
    }
    node.HashTableLoad(&orig_hash_table.front());

    // Create vector of 8 local requests.
    vector<Request> local_reqs = {
        Request {
            .ridx = 1,
            .bidx = 0,
            .pidx = 1,
            .addr = (17 << 11) + 000,
            .weight = 1.0,
            .dest = 17
        },
        Request {
            .ridx = 1,
            .bidx = 0,
            .pidx = 1,
            .addr = (17 << 11) + 100,
            .weight = 1.0,
            .dest = 17
        },
        Request {
            .ridx = 1,
            .bidx = 0,
            .pidx = 1,
            .addr = (17 << 11) + 200,
            .weight = 1.0,
            .dest = 17
        },
        Request {
            .ridx = 1,
            .bidx = 0,
            .pidx = 1,
            .addr = (17 << 11) + 300,
            .weight = 1.0,
            .dest = 17
        },
        Request {
            .ridx = 1,
            .bidx = 0,
            .pidx = 1,
            .addr = (17 << 11) + 400,
            .weight = 1.0,
            .dest = 17
        },
        Request {
            .ridx = 1,
            .bidx = 0,
            .pidx = 1,
            .addr = (17 << 11) + 500,
            .weight = 1.0,
            .dest = 17
        },
        Request {
            .ridx = 1,
            .bidx = 0,
            .pidx = 1,
            .addr = (17 << 11) + 600,
            .weight = 1.0,
            .dest = 17
        },
        Request {
            .ridx = 1,
            .bidx = 0,
            .pidx = 1,
            .addr = (17 << 11) + 700,
            .weight = 1.0,
            .dest = 17
        }
    };

    node.in_left_reqs = local_reqs;

    for (int i = 0; i < 8; ++i) {
        node.Cycle();
        // Log start
        cout << i <<endl;
        cout << "out_down_sums : ";
        for (auto &elem : node.out_down_sums) {
            cout << (unsigned int)elem.ridx << " " << (unsigned int)elem.ridx << endl;
        }
        // Check AIBANode outputs the psum when cnt == 8
        if (i == 7) {
            assert(!node.out_down_sums.empty());
        }
        cout << "psum_buffer " << endl;
        for (auto &elem : node.GetPSumBuffer()) {
            cout << (unsigned int) elem.cnt << " : " << (float) elem.psum.elem0 << " " << (float) elem.psum.elem1 << endl;
            // Check AIBANode correctly add Partial sum
            assert(elem.cnt == i+1);
        }
        // Log end
    }

    return 0;
}