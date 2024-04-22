
/*----------------------------------------------------------------------------*
 * HDR reconstruction for alternating gain (ISO) sensor readout
 * Eurographics 2014 short Paper
 * For more information visit : http://vcl.itn.liu.se/publications/2014/HKU14/
 * Linkoping University
 *----------------------------------------------------------------------------*/



#include <VclMatlab.h>
#include <opencv2/opencv.hpp>
#include <arg.h>
#include <VclExr.h>

#include "VclBayerPatterns.h"
#include "VclKernelRegression.h"
#include "VclShadingCorrection.h"
#include <iostream>
#include <limits>
#include <omp.h>
#include <stdexcept>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// compute the sampling pattern for 

/*----------------------------------------------------------------------------*/	     
inline void CopyPixelData(Eigen::MatrixXf &output,
			  int i, int j, // center pixel
			  vcl::VclBayerPatternMatrix &offets,
			  Eigen::MatrixXf &image);

int main(int argc, char **argv)
{
 
 
  
  char *calibration_file_cptr = NULL;
  char *xml_file_cptr = NULL;
  char *fname_input = NULL;
  char *fname_output = NULL;
  float h = 1.2; 
  int i_fSizeX  = 11, i_fSizeY  = 11;
  int i_CRK = 2; 

  bool isSimulated = false;
  if(arg_parse(argc, argv,
	       "", "dualISO v0.1", 
	       "", "---------------------------------",
	       "", "Usage: %s [options]", argv[0],
	       "%S %S", &fname_input, &fname_output, "\n1. input dualISO frame\n2. output .exr file",
	       "-xml %S" , &xml_file_cptr, "\ncamera xml file rawspeed. " ,
	       "-calibration %S", &calibration_file_cptr, "\ncamera calibration file " ,
	       "-h %f", &h, "kernel h value [default=%f]", h,
	       "-fsizex %d", &i_fSizeX, "Filter size x-dim [default=%d]", i_fSizeX,
	       "-fsizey %d", &i_fSizeY, "Filter size y-dim [default=%d]", i_fSizeY,
	       "-M %d", &i_CRK, "Selects the order of the polynomial (0,1,2) [default = %d]", i_CRK,
	        NULL) < 0) exit(1);

 
 
	 // Load dualISO image  
  std::string fname = fname_input; free(fname_input);
  std::string xml_file = xml_file_cptr; free(xml_file_cptr);
  std::string calibration_fname = calibration_file_cptr; free(calibration_file_cptr);


  /****************************** Shading Correction *******************************/
  std::cout<< "Reading Calibration file.... " <<  std::endl; 

  Vcl::Vclshadingcorrection shadingCorrection;
  if(! shadingCorrection.read_calibration(calibration_fname))
  {
  	std::cout<< "something went wrong!" << std::endl;
  	exit(1);
  }

  if(! shadingCorrection.read_raw_image(fname, xml_file))
  {
  	std::cout<< "something went wrong!" << std::endl;
  	exit(1);
  }

  if(! shadingCorrection.estimate_radiant_pow())
  {
  	std::cout<< "something went wrong!" << std::endl;
  	exit(1);
  }
  
  
  if(i_CRK <0 || i_CRK > 2) {
    std::cout << "Will iterate over M = (0,1,2) order polynomials and choose the smallest error" << std::endl;
  }
 
  cv::Mat output(shadingCorrection.rawdata.rows(), shadingCorrection.rawdata.cols(), CV_32FC3);


  int width = output.cols;
  int height = output.rows;

  // Reconstruct green channel
  vcl::VclBayerPatternMatrix P00;
  vcl::VclBayerPatternMatrix P10;
  vcl::VclBayerPatternMatrix P01;
  vcl::VclBayerPatternMatrix P11;
  
  // 1. Select initial filter size
  int n_channels = 3;
  int channel = 0;


  // White balance for Canon 5D Mark III 
  float white_balance[3] = { 2.125175,0.943985,1.338680};
  
  for(channel; channel < n_channels; channel++)
  {
    float hh = h;
    if(channel == 0)
    {
	    hh *= 2.f;
	    vcl::GetBayerSamplingPatterns(vcl::RGGB_RED, i_fSizeX, i_fSizeY, 
	    P00, P10, P01, P11);
	}
	else if(channel == 1)
	{
	    vcl::GetBayerSamplingPatterns(vcl::RGGB_GREEN, i_fSizeX, i_fSizeY, 
	    P00, P10, P01, P11);
	}
	else
	{
	    vcl::GetBayerSamplingPatterns(vcl::RGGB_BLUE, i_fSizeX, i_fSizeY, 
	    P00, P10, P01, P11);
	    hh *= 2.f;
	}
	  // the different filters generate a different number of samples

	  Eigen::MatrixXf P00f = P00.cast<float>();
	  Eigen::MatrixXf P10f = P10.cast<float>();
	  Eigen::MatrixXf P01f = P01.cast<float>();
	  Eigen::MatrixXf P11f = P11.cast<float>();
	 
	  #pragma omp parallel for schedule(dynamic, 1)
	 for(int j = 0; j < height; j++){
	  
	    Eigen::MatrixXf P00_pixel_data(P00.rows(), 1);
	    Eigen::MatrixXf std_00(P00.rows(), 1);
	    Eigen::MatrixXf W_00(P00.rows(), 1);
	    Eigen::MatrixXf P10_pixel_data(P10.rows(), 1);
	    Eigen::MatrixXf std_10(P10.rows(), 1);
	    Eigen::MatrixXf W_10(P10.rows(), 1);
	    Eigen::MatrixXf P01_pixel_data(P00.rows(), 1);
	    Eigen::MatrixXf std_01(P01.rows(), 1);
	    Eigen::MatrixXf W_01(P01.rows(), 1);
	    Eigen::MatrixXf P11_pixel_data(P10.rows(), 1);
	    Eigen::MatrixXf std_11(P11.rows(), 1);
	    Eigen::MatrixXf W_11(P11.rows(), 1);
	    Eigen::Matrix<float, 6, 1> estimate;
	    Eigen::Matrix<float, 6, 1> estimate_tmp;
	    float M = 0;
	    for(int i = 0; i < width; i++)
	    {
	      //Get pixel region of size (2*i_fSizeX+1, 2*i_fSizeY+1) around pixel (i,j)
	             
			if(j % 2 == 0)
			{ 
			  if(i % 2 == 0)
			  {
			    CopyPixelData(P00_pixel_data, i, j, P00, shadingCorrection.rawdata);
			    CopyPixelData(std_00, i, j, P00, shadingCorrection.variance);
			    CopyPixelData(W_00, i, j, P00, shadingCorrection.weights);
			    if(i_CRK == 2)
			      vcl::CRK2_estimate(estimate, P00_pixel_data, P00f, std_00, W_00, hh);
			    else if(i_CRK == 1)
			      vcl::CRK1_estimate(estimate, P00_pixel_data, P00f, std_00, W_00,  hh);
			    else if(i_CRK == 0)
			      vcl::CRK0_estimate(estimate, P00_pixel_data, P00f, std_00, W_00,  hh);
			    else
			    {
			      M = 2.f;
			      vcl::CRK2_estimate(estimate, P00_pixel_data, P00f, std_00, W_00, hh);
			      vcl::CRK1_estimate(estimate_tmp, P00_pixel_data, P00f, std_00, W_00,  hh);
			      if(estimate(3,0) > estimate_tmp(3,0))
			      {
					estimate = estimate_tmp;
					M = 1.f;
			      }
			      vcl::CRK0_estimate(estimate_tmp, P00_pixel_data, P00f, std_00, W_00,  hh);
			      if(estimate(3,0) > estimate_tmp(3,0))
			      {
					estimate = estimate_tmp;
					M = 0.f;
			      }
			    }
			  }
			  else
			  {
			    CopyPixelData(P10_pixel_data, i, j, P10, shadingCorrection.rawdata);
			    CopyPixelData(std_10, i, j, P10, shadingCorrection.variance); 
			    CopyPixelData(W_10, i, j, P10, shadingCorrection.weights);
			    if(i_CRK == 2)
			      vcl::CRK2_estimate(estimate, P10_pixel_data, P10f, std_10,W_10, hh);
			    else if(i_CRK == 1)
			      vcl::CRK1_estimate(estimate, P10_pixel_data, P10f, std_10,W_10, hh);
			    else if(i_CRK == 0)
			      vcl::CRK0_estimate(estimate, P10_pixel_data, P10f, std_10,W_10, hh);
			    else
			    {
			      M = 2.f;
			      vcl::CRK2_estimate(estimate, P10_pixel_data, P10f, std_10, W_10, hh);
			      vcl::CRK1_estimate(estimate_tmp, P10_pixel_data, P10f, std_10, W_10, hh);
			      if(estimate(3,0) > estimate_tmp(3,0))
			      {
					estimate = estimate_tmp;
					M = 1.f;
			      }
			      vcl::CRK0_estimate(estimate_tmp, P10_pixel_data, P10f, std_10, W_10, hh);
			      if(estimate(3,0) > estimate_tmp(3,0))
			      {
					estimate = estimate_tmp;
					M = 0.f;
			      }
			    }
			  }
			}
			else
			{ 
			  if(i % 2 == 0)
			  {
			    CopyPixelData(P01_pixel_data, i, j, P01, shadingCorrection.rawdata);
			    CopyPixelData(std_01, i, j, P01, shadingCorrection.variance);
			    CopyPixelData(W_01, i, j, P01, shadingCorrection.weights);
			    if(i_CRK == 2)
			      vcl::CRK2_estimate(estimate, P01_pixel_data, P01f, std_01,W_01, hh);
			    else if(i_CRK == 1)
			      vcl::CRK1_estimate(estimate, P01_pixel_data, P01f, std_01,W_01, hh);
			    else if(i_CRK == 0)
			      vcl::CRK0_estimate(estimate, P01_pixel_data, P01f, std_01,W_01, hh);
			    else
			    {
			      M  =2.f;
			      vcl::CRK2_estimate(estimate, P01_pixel_data, P01f, std_01,W_01, hh);
			      vcl::CRK1_estimate(estimate_tmp, P01_pixel_data, P01f, std_01,W_01, hh);
			      if(estimate(3,0) > estimate_tmp(3,0))
			     	{
						estimate = estimate_tmp;
						M = 1.f;
			        }
			      vcl::CRK0_estimate(estimate_tmp, P01_pixel_data, P01f, std_01,W_01, hh);
			      if(estimate(3,0) > estimate_tmp(3,0))
			      {
					estimate = estimate_tmp;
					M = 0.f;
			      }
			    }
			  }
			  else
			  {
			    CopyPixelData(P11_pixel_data, i, j, P11, shadingCorrection.rawdata);
			    CopyPixelData(std_11, i, j, P11, shadingCorrection.variance);
			    CopyPixelData(W_11, i, j, P11, shadingCorrection.weights);
			    if(i_CRK == 2)
			      vcl::CRK2_estimate(estimate, P11_pixel_data, P11f, std_11, W_11, hh);
			    else if(i_CRK == 1)
			      vcl::CRK1_estimate(estimate, P11_pixel_data, P11f, std_11, W_11, hh);
			    else if(i_CRK == 0)
			      vcl::CRK0_estimate(estimate, P11_pixel_data, P11f, std_11, W_11, hh);
			    else
			    {
			      M = 2.f;
			      vcl::CRK2_estimate(estimate, P11_pixel_data, P11f, std_11, W_11, hh);
			      vcl::CRK1_estimate(estimate_tmp, P11_pixel_data, P11f, std_11, W_11, hh);
			      if(estimate(3,0) > estimate_tmp(3,0))
			      {
					estimate = estimate_tmp;
					M = 1.f;
			      }
			      vcl::CRK0_estimate(estimate_tmp, P11_pixel_data, P11f, std_11, W_11, hh);
			      if(estimate(3,0) > estimate_tmp(3,0))
			      {
					estimate = estimate_tmp;
					M = 0.f;
			      }
			    }
			  }
			} //odd rows 
			output.ptr<float>(j)[i*3+channel] = estimate(0,0)* white_balance[channel];
				     
	    } //     for(int i = 0; i < width; i++){
	    if(channel == 0) std::cout << "Red Line: " << j << std::endl;
	    else if(channel == 1) std::cout << "Green Line: " << j << std::endl;
	    else if(channel == 2) std::cout << "Blue Line: " << j << std::endl;
	  } //  for(mint j = 0; j < height; j++){
  } // for channel
 
  std::cout << "Image reconstruction done" << std::endl; 
  
  Vcl::VclExr vclexr(output);
  Vcl::VclExr::setThreadCount(12);
  fname = std::string(fname_output);
  vclexr.writeExrRgbFloat(fname);
  
  


}


/*----------------------------------------------------------------------------*/
 void CopyPixelData(Eigen::MatrixXf &output,
		    int i, int j, //(i,j) pixel position, c color channel  
		    vcl::VclBayerPatternMatrix &offsets,//relative pixel positions
		    Eigen::MatrixXf &image)
 {
   if(output.rows() != offsets.rows())
   {
     output.resize(offsets.rows(), 1);
     
   }
   int width = image.cols();
   int height = image.rows();
   for(int k = 0; k < offsets.rows(); k++)
   {
     int x = i + offsets(k,0); x = (x < 0) ? 0 : ((x >= width) ? width - 1: x); 
     int y = j + offsets(k,1); y = (y < 0) ? 0 : ((y >= height) ? height - 1: y); 
     output(k,0) = image(y,x);
   }
 }


