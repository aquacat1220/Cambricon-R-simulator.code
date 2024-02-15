#ifndef AIBA_COMMON
#define AIBA_COMMON

/**
 * @brief Struct representing sample point in 3D space.
 * 
 */
struct Point {
    /**
     * @brief The ray index. 0 for the top-left pixel's ray, counts up right and down.
     * 
     */
    unsigned int ridx;
    /**
     * @brief The batch index. 0 for the first batch of the ray, and 7 at max.
     * 
     */
    unsigned char bidx;
    /**
     * @brief The point index. 0 for the first point of the batch, and 31 at max.
     * 
     */
    unsigned char pidx;
    float x;
    float y;
    float z;
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
    /**
     * @brief The ray index. 0 for the top-left pixel's ray, counts up right and down.
     * 
     */
    unsigned int ridx;
    /**
     * @brief The batch index. 0 for the first batch of the ray, and 7 at max.
     * 
     */
    unsigned char bidx;
    /**
     * @brief The point index. 0 for the first point of the batch, and 31 at max.
     * 
     */
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
    /**
     * @brief The ray index. 0 for the top-left pixel's ray, counts up right and down.
     * 
     */
    unsigned int ridx;
    /**
     * @brief The batch index. 0 for the first batch of the ray, and 7 at max.
     * 
     */
    unsigned char bidx;
    /**
     * @brief The point index. 0 for the first point of the batch, and 31 at max.
     * 
     */
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
    /**
     * @brief The ray index. 0 for the top-left pixel's ray, counts up right and down.
     * 
     */
    unsigned int ridx;
    /**
     * @brief The batch index. 0 for the first batch of the ray, and 7 at max.
     * 
     */
    unsigned char bidx;
    /**
     * @brief The point index. 0 for the first point of the batch, and 31 at max.
     * 
     */
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
    /**
     * @brief The ray index. 0 for the top-left pixel's ray, counts up right and down.
     * 
     */
    unsigned int ridx;
    /**
     * @brief The batch index. 0 for the first batch of the ray, and 7 at max.
     * 
     */
    unsigned char bidx;
    /**
     * @brief The point index. 0 for the first point of the batch, and 31 at max.
     * 
     */
    unsigned char pidx;
    // Accumulated sum
    HashEntry sum;
};

#endif