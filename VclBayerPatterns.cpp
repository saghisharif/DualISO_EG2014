#include "VclBayerPatterns.h"
#include <iostream>

using namespace Eigen;

/*----------------------------------------------------------------------------*/
namespace vcl{

  
  /*--------------------------------------------------------------------------*
   * The computation of the patterns are 
   *--------------------------------------------------------------------------*/
  
  
  int  GetBayerSamplingPatterns(BAYER_PATTERN CFA, int sizeX, int sizeY,
				VclBayerPatternMatrix &P00,
				VclBayerPatternMatrix &P10,
				VclBayerPatternMatrix &P01,
				VclBayerPatternMatrix &P11)
  {

  if(!P00.IsRowMajor || !P10.IsRowMajor || !P01.IsRowMajor || !P11.IsRowMajor){
    // Note: I bleive this should be taken care of already in the parameter 
    // definition in the function call GetBaryerSamplingPatterns
    std::cout << "vcl::GetBayerPatternsGreen - Error:"  << std::endl;
    std::cout << "Input matrices must be Eigen::RowMajor" << std::endl; 
    std::cout << "Will EXIT with empty matrices!" << std::endl;
    return 1;
  }
    
  switch(CFA){
  case RGGB_RED:
    RGGB_red_bayer_patterns(sizeX, sizeY, P00, P10, P01, P11);
    break;
  case RGGB_GREEN:
    RGGB_green_bayer_patterns(sizeX, sizeY, P00, P10, P01, P11);
    break;
  case RGGB_BLUE:
    RGGB_blue_bayer_patterns(sizeX, sizeY, P00, P10, P01, P11);
    break;
    
  default: break;
  };
  return 0;
}


  
  /*--------------------------------------------------------------------------*
   * Computation of RGGB patterns
   *--------------------------------------------------------------------------*/
  
  
  void RGGB_red_bayer_patterns(int sizeX, int sizeY,
			       VclBayerPatternMatrix &P00,
			       VclBayerPatternMatrix &P10,
			       VclBayerPatternMatrix &P01,
			       VclBayerPatternMatrix &P11)
  {
      
    int shiftY = (sizeY > sizeX) ? (sizeY - sizeX) % 2 : (sizeX - sizeY) % 2;
    int vec_size = (sizeY + 1 - sizeY % 2) * (sizeX +1 - sizeX % 2); 
    P00.resize(vec_size, 2);
    vec_size = (sizeY + sizeY % 2) * (sizeX + sizeX % 2); 
    P11.resize(vec_size, 2);
    vec_size = (sizeY + (1 - sizeY % 2)) * (sizeX + sizeX % 2);  
    P10.resize(vec_size, 2);
    vec_size = (sizeX + (1 - sizeX % 2)) * (sizeY + sizeY % 2);  
    P01.resize(vec_size, 2);
    
    int P00_idx = 0;
    int P10_idx = 0;
    int P01_idx = 0;
    int P11_idx = 0;
    int acc = 0;
    for(int j =-sizeY; j <= sizeY; j++){
      if(j % 2 == 0){ // even rows
	for(int i = -sizeX ; i <= sizeX; i++){
	  if(i % 2 == 0){// even columns
	    P00(P00_idx, 0) = i; P00(P00_idx++, 1) = j;
	  }else{ // odd columns
	    P10(P10_idx, 0) = i; P10(P10_idx++, 1) = j;
	  }
	} // for( i = 0; ...
      }else{ // odd rows
	for(int i = -sizeX; i <= sizeX; i++){
	  if(i % 2 == 0){// even columns
	    P01(P01_idx, 0) = i; P01(P01_idx++, 1) = j;
	  }else{ // odd columns
	    P11(P11_idx, 0) = i; P11(P11_idx++, 1) = j;
	  }
	} // for(i = 0; ... 
      } // else // odd rows
    } // for(j = 0; ...
  }
    
  




  void RGGB_green_bayer_patterns(int sizeX, int sizeY,
				 VclBayerPatternMatrix &P00,
				 VclBayerPatternMatrix &P10,
				 VclBayerPatternMatrix &P01,
				 VclBayerPatternMatrix &P11)
  {
    
    int shiftY = (sizeY > sizeX) ? (sizeY - sizeX) % 2 : (sizeX - sizeY) % 2;
    // P00 and P11 has the same configuration and number of green samples
    int vec_size = (sizeY + 1) * sizeX + sizeY * (sizeX + 1) + shiftY;
    // std::cout << "P00 size: " << sizeX << ", " << sizeY << ", "<< vec_size << std::endl;
    P00.resize(vec_size, 2);
    P11.resize(vec_size, 2);
    // P10 and P01 has the same configuration and number of green samples
    vec_size = (sizeY + 1) * sizeX + sizeY * (sizeX + 1) + (1 - shiftY);
    // std::cout << "P10 size: " << sizeX << ", " << sizeY << ", "<< vec_size << std::endl;
    P10.resize(vec_size, 2);
    P01.resize(vec_size, 2);
    
    int P00_idx = 0;
    int P10_idx = 0;
    int P01_idx = 0;
    int P11_idx = 0;
    
    for(int j =-sizeY; j <= sizeY; j++){
      if(j % 2 == 0){ // even rows
	for(int i = -sizeX ; i <= sizeX; i++){
	  if(i % 2 == 0){// even columns
	    // std::cout << " R "; //P00 
	    // std::cout << " G "; // P10
	    //std::cout << "P10 idx: " << P10_idx << std::endl;
	    P10(P10_idx, 0) = i; P10(P10_idx++, 1) = j;
	    P01(P01_idx, 0) = i; P01(P01_idx++, 1) = j;
	  }else{ // odd columns
	    // std::cout << " G "; //P00
	    // std::cout << " R "; // P10 
	    // std::cout << "idx: " << P00_idx << std::endl;
	    P00(P00_idx, 0) = i; P00(P00_idx++, 1) = j;
	    P11(P11_idx, 0) = i; P11(P11_idx++, 1) = j;
	  }
	} // for( i = 0; ...
      }else{ // odd rows
	for(int i = -sizeX; i <= sizeX; i++){
	  if(i % 2 == 0){// even columns
	    // std::cout << " G "; //P00
	    // std::cout << " B "; // P10   
	    //std::cout << "idx: " << P00_idx << std::endl;
	    P00(P00_idx, 0) = i; P00(P00_idx++, 1) = j;
	    P11(P11_idx, 0) = i; P11(P11_idx++, 1) = j;
	  }else{ // odd columns
	    // std::cout << " B "; //P00
	    // std::cout << " G "; // P10    
	    //std::cout << "P10 idx: " << P10_idx << std::endl;
	    P10(P10_idx, 0) = i; P10(P10_idx++, 1) = j;
	    P01(P01_idx, 0) = i; P01(P01_idx++, 1) = j;
	  }
	} // for(i = 0; ... 
      } // else // odd rows
      //std::cout << std::endl;
    } // for(j = 0; ...
    //std::cout << "Sum P00: " << accP00 << ", Sum P10: " << accP10 << std::endl;
  }




  void RGGB_blue_bayer_patterns(int sizeX, int sizeY,
				VclBayerPatternMatrix &P00,
				VclBayerPatternMatrix &P10,
				VclBayerPatternMatrix &P01,
				VclBayerPatternMatrix &P11)
  {
    RGGB_red_bayer_patterns(sizeX, sizeY, P11, P01, P10, P00);
  }

}; // namespace vcl
