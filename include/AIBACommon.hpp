#ifndef AIBA_COMMON
#define AIBA_COMMON

struct Point {
    float x, y, z;
};

struct Request {
    // Request Index
    unsigned char ridx;
    // Point Index
    unsigned char pidx;
    // Address to read
    unsigned int addr;
    // Interpolation weight of hash entry
    float weight;
    // Destination node for accumulation
    unsigned char dest;
};

struct Packet {
    // Request Index
    unsigned char ridx;
    // Point Index
    unsigned char pidx;
    // Interpolated data
    float data;
    // Destination node for accumulation
    unsigned char dest;
};

struct PSum {
    // Request Index
    unsigned char ridx;
    // Point Index
    unsigned char pidx;
    // Accumulated partial sum
    float psum;
    // Count of accumulated vertices
    unsigned int cnt;
};

struct Sum {
    // Request Index
    unsigned char ridx;
    // Point Index
    unsigned char pidx;
    // Accumulated partial sum
    float sum;
};

#endif