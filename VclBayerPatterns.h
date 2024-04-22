#ifndef __VCL_BAYER_PATTERH_H__
#define __VCL_BAYER_PATTERH_H__

#include <eigen3/Eigen/Dense>

/*-----------------------------------------------------------------------------* 
 * Convenience functions for computing sampling patterns for various Bayer
 * pattern configurations. 
 * The sampling patterns stored in the Eigen matrices (1D vectors) are relative.
 * For a given sampling point X in an image, the indended use is:
 * Y = X + Pxy, 
 * where Y are the pixel coordinates to sample for a specific color channel 
 * given the size of a filter support (int sizeX, int sizeY).
 * int CFA is the Bayer pattern configuration as described by the enumerator:
 * enum BAYER_PATTERN below.
 *-----------------------------------------------------------------------------*/

namespace vcl{
  typedef Eigen::Matrix<int, Eigen::Dynamic, 2, Eigen::RowMajor>
    VclBayerPatternMatrix;

  enum BAYER_PATTERN{
    RGGB_RED = 1, // first row RGRGRGRG, second row GBGBGBGB
    RGGB_GREEN,
    RGGB_BLUE
  };

  // Sampling Bayer patterns as specified by CFA (Bayer configuration and
  // color channel) and filter support.
  // This function is mainly for convenience as it calls the specific
  // functions below depending on the parameters.
  int GetBayerSamplingPatterns(BAYER_PATTERN CFA, int sizeX, int sizeY,
			       VclBayerPatternMatrix &P00,
			       VclBayerPatternMatrix &P10,
			       VclBayerPatternMatrix &P01,
			       VclBayerPatternMatrix &P11);

  void RGGB_red_bayer_patterns(int sizeX, int sizeY,
				 VclBayerPatternMatrix &P00,
				 VclBayerPatternMatrix &P10,
				 VclBayerPatternMatrix &P01,
				 VclBayerPatternMatrix &P11);


  void RGGB_green_bayer_patterns(int sizeX, int sizeY,
				 VclBayerPatternMatrix &P00,
				 VclBayerPatternMatrix &P10,
				 VclBayerPatternMatrix &P01,
				 VclBayerPatternMatrix &P11);

  void RGGB_blue_bayer_patterns(int sizeX, int sizeY,
				VclBayerPatternMatrix &P00,
				VclBayerPatternMatrix &P10,
				VclBayerPatternMatrix &P01,
				VclBayerPatternMatrix &P11);
  

 
}; // namespace vcl{


#endif // __VCL_BAYER_PATTERH_H__
