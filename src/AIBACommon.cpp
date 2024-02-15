#include "AIBACommon.hpp"


bool operator==(const Point& lhs, const Point& rhs) {
    return ((lhs.ridx == rhs.ridx) && (lhs.bidx == rhs.bidx) && (lhs.pidx == rhs.pidx) && \
    (lhs.x == rhs.x) && (lhs.y == rhs.y) && \
    (lhs.z == rhs.z));
}

bool operator<(const Point& lhs, const Point& rhs) {
    return ((lhs.ridx < rhs.ridx) || (lhs.bidx < rhs.bidx) || (lhs.pidx < rhs.pidx) || \
    (lhs.x < rhs.x) || (lhs.y < rhs.y) || \
    (lhs.z < rhs.z));
}

bool operator==(const HashEntry& lhs, const HashEntry& rhs)
{
    return ((lhs.elem0 == rhs.elem0) && (lhs.elem1 == rhs.elem1));
}


bool operator<(const HashEntry& lhs, const HashEntry& rhs) {
    return ((lhs.elem0 < rhs.elem0) || (lhs.elem1 < rhs.elem1));
}

bool operator==(const Request& lhs, const Request& rhs) {
    return (lhs.ridx == rhs.ridx) && (lhs.bidx == rhs.bidx) && (lhs.pidx == rhs.pidx) && (lhs.addr == rhs.addr) && \
    (lhs.weight == rhs.weight) && (lhs.dest == rhs.dest);
}


bool operator<(const Request& lhs, const Request& rhs) {
    return (lhs.ridx < rhs.ridx) || (lhs.bidx < rhs.bidx) || (lhs.pidx < rhs.pidx) || (lhs.addr < rhs.addr) || \
    (lhs.weight < rhs.weight) || (lhs.dest < rhs.dest);
}

bool operator==(const Packet& lhs, const Packet& rhs) {
    return (lhs.ridx == rhs.ridx) && (lhs.bidx == rhs.bidx) && (lhs.pidx == rhs.pidx) && (lhs.data == rhs.data) && \
    (lhs.dest == rhs.dest);
}


bool operator<(const Packet& lhs, const Packet& rhs) {
    return (lhs.ridx < rhs.ridx) || (lhs.bidx < rhs.bidx) || (lhs.pidx < rhs.pidx) || (lhs.data < rhs.data) || \
    (lhs.dest < rhs.dest);
}