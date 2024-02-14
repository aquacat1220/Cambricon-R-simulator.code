#include "AIBANode.hpp"

int main() {
    AIBANode node = AIBANode(0);
    vector<HashEntry> orig_hash_table;
    // Fill in orig_hash_table with 2^11 length HashEntry array.
    orig_hash_table.assign((1 << 11),{ 1.0f, 1.0f });
    orig_hash_table[1827] = { .elem0 = 356.0f, .elem1 = 357.0f };
    // Load it into our AIBA node.
    node.HashTableLoad(&orig_hash_table.front());
    // Retrieve it, and check if its the same as our original hash table!
    auto retr_hash_table = node.GetSramBank();
    return !(orig_hash_table == retr_hash_table);
}