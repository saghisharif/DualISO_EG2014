
#include "VclKernelRegression.h"
#include <iostream>
/*----------------------------------------------------------------------------*
 * For more information visit : http://vcl.itn.liu.se/publications/2014/HKU14/
 * Linkoping University
 *----------------------------------------------------------------------------*/

namespace vcl{
  
  void CRK0_estimate(Eigen::Matrix<float, 6, 1> &estimate,
         const Eigen::MatrixXf &data,
         const Eigen::MatrixXf &samplePos,
         const Eigen::MatrixXf &variances,
         const Eigen::MatrixXf &weights,
         const float h)
  {
    
    Eigen::MatrixXf tt((-0.5f/(h*h))*((samplePos.col(0).array().pow(2)  + 
                         samplePos.col(1).array().pow(2)))); 

    Eigen::MatrixXf weightsi = weights / weights.sum();
    Eigen::MatrixXf W = (tt.array().array().exp() * weightsi.array()) / (sqrtf(2.f *3.141593f)*h*h);
    float ww  = W.sum();
    
    float val = (W.transpose() * data)(0) / ww;
   
    estimate(0,0) = val;
    
  }

  void CRK1_estimate(Eigen::Matrix<float, 6, 1> &estimate,
         const Eigen::MatrixXf &data,
         const Eigen::MatrixXf &samplePos,
         const Eigen::MatrixXf &variances,
         const Eigen::MatrixXf &weights,
         const float h){
 
    Eigen::MatrixXf tt((-0.5f/(h*h))*((samplePos.col(0).array().pow(2)  + samplePos.col(1).array().pow(2)).cast<float>()));
    tt.array() = tt.array().exp()  / (sqrtf(2.f *3.141593f)*h*h);
   
    Eigen::MatrixXf W = (tt.array()) * weights.array();
   
    // Equivalent kernel
    Eigen::MatrixXf Xx(samplePos.rows(), 3);
    Xx.col(0).setOnes();
    Xx.col(1) = samplePos.col(0);
    Xx.col(2) = samplePos.col(1);

    Eigen::MatrixXf Xw(samplePos.rows(), 3);
    Xw.col(0) = W;
    Xw.col(1) = samplePos.col(0).array() * W.array();
    Xw.col(2) = samplePos.col(1).array() * W.array();
    
    Eigen::MatrixXf A = Xx.transpose() * Xw;
    // solve for coefficients
    Eigen::VectorXf y = (Xw.transpose() * data);
    Eigen::MatrixXf coeff = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(y);

    estimate(0,0) = coeff(0,0);
    
  }



  void CRK2_estimate(Eigen::Matrix<float, 6, 1> &estimate,
         const Eigen::MatrixXf &data,
         const Eigen::MatrixXf &samplePos,
         const Eigen::MatrixXf &variances,
         const Eigen::MatrixXf &weights,
         const float h){

    Eigen::MatrixXf 
      tt((-0.5f/(h*h))*((samplePos.col(0).array().pow(2)  + 
       samplePos.col(1).array().pow(2))));
    tt.array() = tt.array().exp()  / (sqrtf(2.f *3.141593f)*h*h);
  
   
    Eigen::MatrixXf W = (tt.array()) * weights.array();
   
    // Equivalent kernel
    Eigen::MatrixXf Xx(samplePos.rows(), 6);
    Xx.col(0).setOnes();
    Xx.col(1) = samplePos.col(0);
    Xx.col(2) = samplePos.col(1);
    Xx.col(3) = samplePos.col(0).array().pow(2);
    Xx.col(4) = samplePos.col(0).array()* samplePos.col(1).array();
    Xx.col(5) = samplePos.col(1).array().pow(2);

    Eigen::MatrixXf Xw(samplePos.rows(), 6);
    Xw.col(0) = W;
    Xw.col(1) = Xx.col(1).array() * W.array();
    Xw.col(2) = Xx.col(2).array() * W.array();
    Xw.col(3) = Xx.col(3).array() * W.array();
    Xw.col(4) = Xx.col(4).array() * W.array();
    Xw.col(5) = Xx.col(5).array() * W.array();
    
    Eigen::MatrixXf A = Xx.transpose() * Xw;
    // solve for coefficients
    Eigen::VectorXf y = (Xw.transpose()) * data;
    Eigen::MatrixXf coeff = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(y);

   
    estimate(0,0) = coeff(0,0);
     
  }


}; //namepsace vcl
