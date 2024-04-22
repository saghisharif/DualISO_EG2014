#ifndef __VCL_KERNEL_REGRESSION__
#define __VCL_KERNEL_REGRESSION__

#include <eigen3/Eigen/Dense>

/*----------------------------------------------------------------------------*
 * kernel regression convolved with camera noise model estimation.
 * For more information visit : http://vcl.itn.liu.se/publications/2014/HKU14/
 * Linkoping University
 *----------------------------------------------------------------------------*/

namespace vcl{


  void CRK0_estimate(Eigen::Matrix<float, 6, 1> &estimate,
		     const Eigen::MatrixXf &data,
		     const Eigen::MatrixXf &samplePos,
		     const Eigen::MatrixXf &variances,
		     const Eigen::MatrixXf &weights,
		     const float h);
 
  void CRK1_estimate(Eigen::Matrix<float, 6, 1> &estimate,
		     const Eigen::MatrixXf &data,
		     const Eigen::MatrixXf &samplePos,
		     const Eigen::MatrixXf &variances,
		     const Eigen::MatrixXf &weights,
		     const float h);

  
  
  void CRK2_estimate(Eigen::Matrix<float, 6, 1> &estimate,
		     const Eigen::MatrixXf &data,
		     const Eigen::MatrixXf &samplePos,
		     const Eigen::MatrixXf &variances,
		     const Eigen::MatrixXf &weights,
		     const float h);
    
  
}; // namespace vcl


#endif //  __VCL_KERNEL_REGRESSION__
