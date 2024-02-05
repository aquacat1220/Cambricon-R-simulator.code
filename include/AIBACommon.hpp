#ifndef AIBA_COMMON
#define AIBA_COMMON

/**
 * @brief Struct representing sample point in 3D space.
 * 
 */
struct Point {
    float x, y, z;
};

bool operator==(const Point& lhs, const Point& rhs);

bool operator<(const Point& lhs, const Point& rhs);

/**
 * @brief Representing entry in the hash table.
 * Maybe it should be array of half precision floats...
 * 
 */
struct HashEntry {
    float elem0;
    float elem1;
};

bool operator==(const HashEntry& lhs, const HashEntry& rhs);

bool operator<(const HashEntry& lhs, const HashEntry& rhs);

/**
 * @brief Struct representing hash entry read request.
 * 
 */
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

bool operator==(const Request& lhs, const Request& rhs);

bool operator<(const Request& lhs, const Request& rhs);

/**
 * @brief Struct representing read hash entry data.
 * 
 */
struct Packet {
    // Request Index
    unsigned char ridx;
    // Point Index
    unsigned char pidx;
    // Interpolated data
    HashEntry data;
    // Destination node for accumulation
    unsigned char dest;
};

bool operator==(const Packet& lhs, const Packet& rhs);

bool operator<(const Packet& lhs, const Packet& rhs);

/**
 * @brief Struct representing accumulating partial sum.
 * 
 */
struct PSum {
    // Request Index
    unsigned char ridx;
    // Point Index
    unsigned char pidx;
    // Accumulated partial sum
    HashEntry psum;
    // Count of accumulated vertices
    unsigned int cnt;
};

/**
 * @brief Struct representing final interpolated feature.
 * 
 */
struct Sum {
    // Request Index
    unsigned char ridx;
    // Point Index
    unsigned char pidx;
    // Accumulated sum
    HashEntry sum;
};

#endif