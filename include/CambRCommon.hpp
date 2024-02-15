#ifndef CAMB_R_COMMON
#define CAMB_R_COMMON

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
    float feature_vector[48];
};

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
#endif