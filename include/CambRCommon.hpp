#ifndef CAMB_R_COMMON
#define CAMB_R_COMMON

#include <vector>

using namespace std;

/**
 * @brief Struct representing a single ray cast to render the image.
 * 
 */
struct Ray {
    /**
     * @brief The ray index. 0 for the top-left pixel's ray, counts up right and down.
     * 
     */
    unsigned int ridx;
    float x;
    float y;
    float z;
    float theta;
    float phi;
};

bool operator==(const Ray& lhs, const Ray& rhs);

bool operator<(const Ray& lhs, const Ray& rhs);

/**
 * @brief Struct representing a sampled point on a ray.
 * 
 */
struct Sample {
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
    float theta;
    float phi;
};

bool operator==(const Sample& lhs, const Sample& rhs);

bool operator<(const Sample& lhs, const Sample& rhs);

/**
 * @brief Struct representing an encoded feature vector output by the encoding unit.
 * 
 */
struct Feature {
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
    vector<float> feature_vector;
};

bool operator==(const Feature& lhs, const Feature& rhs);

bool operator<(const Feature& lhs, const Feature& rhs);

/**
 * @brief Struct representing a pixel in the inferred image.
 * Each pixel corresponds to a single ray.
 * 
 */
struct Pixel {
    /**
     * @brief The ray index. 0 for the top-left pixel's ray, counts up right and down.
     * 
     */
    unsigned int ridx;
    float r;
    float g;
    float b;
};

bool operator==(const Pixel& lhs, const Pixel& rhs);

bool operator<(const Pixel& lhs, const Pixel& rhs);

struct dram_message_t {
    bool is_write;
    unsigned long address;
    // DRAM_FETCH_SIZE = 64 , FP16 = 2
    float data[64 / 2];
};
#endif