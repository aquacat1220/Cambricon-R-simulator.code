#include "CambRCommon.hpp"


bool operator==(const Ray& lhs, const Ray& rhs) {
    return ((lhs.ridx == rhs.ridx) && (lhs.x == rhs.x) && (lhs.y == rhs.y) && \
    (lhs.z == rhs.z) && (lhs.theta == rhs.theta) && (lhs.phi == rhs.phi));
}

bool operator<(const Ray& lhs, const Ray& rhs) {
    return ((lhs.ridx < rhs.ridx) || (lhs.x < rhs.x) || (lhs.y < rhs.y) || \
    (lhs.z < rhs.z) || (lhs.theta < rhs.theta) || (lhs.phi < rhs.phi));
}

bool operator==(const Sample& lhs, const Sample& rhs) {
    return ((lhs.ridx == rhs.ridx) && (lhs.bidx == rhs.bidx) && (lhs.pidx == rhs.pidx) && \
    (lhs.x == rhs.x) && (lhs.y == rhs.y) && \
    (lhs.z == rhs.z) && (lhs.theta == rhs.theta) && (lhs.phi == rhs.phi));
}

bool operator<(const Sample& lhs, const Sample& rhs) {
    return ((lhs.ridx < rhs.ridx) || (lhs.bidx < rhs.bidx) || (lhs.pidx < rhs.pidx) || \
    (lhs.x < rhs.x) || (lhs.y < rhs.y) || \
    (lhs.z < rhs.z) || (lhs.theta < rhs.theta) || (lhs.phi < rhs.phi));
}

bool operator==(const Feature& lhs, const Feature& rhs) {
    return ((lhs.ridx == rhs.ridx) && (lhs.bidx == rhs.bidx) && (lhs.pidx == rhs.pidx) && \
    (lhs.feature_vector == rhs.feature_vector));
}

bool operator<(const Feature& lhs, const Feature& rhs) {
    return ((lhs.ridx < rhs.ridx) || (lhs.bidx < rhs.bidx) || (lhs.pidx < rhs.pidx) || \
    (lhs.feature_vector < rhs.feature_vector));
}

bool operator==(const Pixel& lhs, const Pixel& rhs) {
    return ((lhs.ridx == rhs.ridx) && \
    (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b));
}

bool operator<(const Pixel& lhs, const Pixel& rhs) {
    return ((lhs.ridx < rhs.ridx) || \
    (lhs.r < rhs.r) || (lhs.g < rhs.g) || (lhs.b < rhs.b));
}