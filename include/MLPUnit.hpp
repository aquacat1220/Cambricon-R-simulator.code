#ifndef MLP_UNIT
#define MLP_UNIT

#include <vector>

#include "CambRCommon.hpp"

using namespace std;

class MlpUnit {
    public:
    MlpUnit(void);

    public:
    // Inputs
    /**
     * @brief Vector of 32 encoded features from the same batch
     * Cleared and refilled when remain_cycle = 0
    */
    vector<Feature> in_features;

    // Outputs
    /**
     * @brief Final pixel color of the corresponding ray
     * 
    */
   Pixel out_pixel;

   /**
    * @brief ridx of the ray processed by this MLP Unit
   */
   unsigned int ridx;

   /**
    * @brief View direction Theta & Phi
    * Projected onto the first 16 coefficients of the spherical harmonics basis
    * Cleared and refilled when computed ray is changed.
   */
   float theta;
   float phi;

   /**
    * @brief Dendity threshold value for early termination
    * Suppose that this value is fixed value stored in ROM. 
   */
   float threshold;

   public:
   /**
    * @brief Compute density and color throuh density network and color network
    * This function should be called every cycle.
    * Computes a single batch untill remain_cycle_=0 .
    * Cycle() doesn't magically propagate the outputs to the global buffer. This is a responsibility of the system.
   */
   void Cycle();
   /**
    * @brief Laods the MLP weights to MLP unit
    * Since we consider only inference, this function called only one time initially by the system  
    * MLP weight consists of several matrices (32x64, 64x16 for density network & 32x64, 64x64, 64x3 for color network)
   */
   void MlpWeightLoad();

   private:
   /**
    * @brief Remained cycles for a single batch MLP computation
    * If this variable != 0, MLP unit cannot proceed another batch
   */
   int remain_cycle_; 

   /**
    * @brief MLP weights for MLP
    * (32x64, 64x16 for density network & 32x64, 64x64, 64x3 for color network)
   */
   vector<vector<float>> w1_density_;
   vector<vector<float>> w2_density_;
   vector<vector<float>> w1_color_;
   vector<vector<float>> w2_color_;
   vector<vector<float>> w3_color_;
   
   /**
    * @brief Save the density and color values of the sample points of the corresponding ray.
    * 
   */
   vector<float> densities_;
   vector<vector<float>> colors_;

   /**
    * @brief For checking whether density over threshold or not
   */
   float accumulate_dens_;

   private:
   /**
    * @brief Clears all inputs
    * 
   */
   void ClearInputs();

   //Spherical harmonic func

   /**
    * @brief Compute final color. (Formula in NeRF paper)
    * 
   */
   vector<float> ComputeColor();
};

#endif