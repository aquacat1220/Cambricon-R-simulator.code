#include "AIBACommon.hpp"

bool operator==(const HashEntry& lhs, const HashEntry& rhs)
{
    return ((lhs.elem0 == rhs.elem0) && (lhs.elem1 == rhs.elem1));
}


bool operator<(const HashEntry& lhs, const HashEntry& rhs) {
    return ((lhs.elem0 < rhs.elem0) || (lhs.elem1 < rhs.elem1));
}

bool operator==(const Request& lhs, const Request& rhs) {
    return (lhs.ridx == rhs.ridx) && (lhs.pidx == rhs.pidx) && (lhs.addr == rhs.addr) && \
    (lhs.weight == rhs.weight) && (lhs.dest == rhs.dest);
}


bool operator<(const Request& lhs, const Request& rhs) {
    return (lhs.ridx < rhs.ridx) || (lhs.pidx < rhs.pidx) || (lhs.addr < rhs.addr) || \
    (lhs.weight < rhs.weight) || (lhs.dest < rhs.dest);
}

bool operator==(const Packet& lhs, const Packet& rhs) {
    return (lhs.ridx == rhs.ridx) && (lhs.pidx == rhs.pidx) && (lhs.data == rhs.data) && \
    (lhs.dest == rhs.dest);
}


bool operator<(const Packet& lhs, const Packet& rhs) {
    return (lhs.ridx < rhs.ridx) || (lhs.pidx < rhs.pidx) || (lhs.data < rhs.data) || \
    (lhs.dest < rhs.dest);
}