#include <VclMatlab.h>
#include "VclShadingCorrection.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include <VclRaw.h>


/*----------------------------------------------------------------------------*
 * Reading the camera calibration data and computes an estimate of the incident 
 * radiant power from noisy digital input sample values 
 * For more information visit : http://vcl.itn.liu.se/publications/2014/HKU14/
 * Linkoping University
 *----------------------------------------------------------------------------*/

namespace Vcl{

Vclshadingcorrection::Vclshadingcorrection()
{

	// Parameters for Canon Mark III
	metadata.i_nBits = 14;
	metadata.i_exposure_t = 1;
	metadata.i_G1Pattern[0] = 0; metadata.i_G1Pattern[1] = 1;
	metadata.i_BPattern[0]  = 0; metadata.i_BPattern[1]  = 0;
	metadata.i_RPattern[0]  = 1; metadata.i_RPattern[1]  = 1;
	metadata.i_G2Pattern[0] = 1; metadata.i_G2Pattern[1] = 0;
	metadata.i_na =1;
	metadata.i_saturation_point = 15282;



}


bool Vclshadingcorrection::read_calibration(std::string fname)
{
	Vcl::VclMatio matlab_input;
  if(!matlab_input.openForReading(fname)){
    std::cout << "Error cannot open file: " << fname << std::endl;
    return false;
  }
  
  
  if(!matlab_input.readEigenMatrix2dNamed("blackpoint", metadata.e_blackpoints))
  {
    std::cout << "Error cannot find matrix named: " << "blackpoint" << std::endl;
    matlab_input.close();
    return false;
  }
  std::cout<< "image blackpoint was read successfully, the dimension: " << metadata.e_blackpoints.rows() << " x " << metadata.e_blackpoints.cols() << std::endl; 
  if(!matlab_input.readEigenMatrix2dNamed("gain_s", metadata.e_scaling_gain))
  {
    std::cout << "Error cannot find matrix named: " << "gain_s" << std::endl;
    matlab_input.close();
    return false;
  }
  std::cout<< "image gain_s was read successfully, the dimension: " << metadata.e_scaling_gain.rows() << " x " << metadata.e_scaling_gain.cols() << std::endl; 
  
  if(!matlab_input.readEigenMatrix2dNamed("gain_dualiso", metadata.e_dualiso_gain))
  {
    std::cout << "Error cannot find matrix named: " << "gain_dualiso" << std::endl;
    matlab_input.close();
    return false;
  }
  std::cout<< "image gain_dualiso was read successfully, the dimension: " << metadata.e_dualiso_gain.rows() << " x " << metadata.e_dualiso_gain.cols() << std::endl; 
  
  if(!matlab_input.readEigenMatrix2dNamed("sr", metadata.e_sr))
  {
    std::cout << "Error cannot find matrix named: " << "sr" << std::endl;
    matlab_input.close();
    return false;
  }
   std::cout<< "image sr was read successfully, the dimension: " << metadata.e_sr.rows() << " x " << metadata.e_sr.cols() << std::endl; 
  

  std::cout << "Read the calibration file sucessfully." << std::endl;
  matlab_input.close();
  return true;

}

bool Vclshadingcorrection::read_raw_image(std::string fname , std::string xml_fname)
{
  cv::Mat cv_raw_image;	
 try
  {
  	cv_raw_image = Vcl::loadRawImage(fname, xml_fname);	
  }
  catch(std::exception &e)
  {
  	std::cout << "Error while reading RAW image file! " << std::endl << e.what() << std::endl;
  	return false;
  }
  Eigen::MatrixXf tmpData;
  cv::cv2eigen(cv_raw_image,tmpData ); 
  
  metadata.i_width  = cv_raw_image.cols - 120-2;
  metadata.i_height =  cv_raw_image.rows - 78;
  
  rawdata = tmpData.block(78,120,metadata.i_height,metadata.i_width);
  
  std::cout<< "image " << fname << " was read successfully, the dimension: " << rawdata.rows() << " x " << rawdata.cols() << std::endl; 
  metadata.i_width  = rawdata.cols();
  metadata.i_height =  rawdata.rows();
  return true;

}


bool Vclshadingcorrection::estimate_radiant_pow()
{


	Eigen::MatrixXf camera_rawdata = rawdata;
	Eigen::MatrixXf e_tmp_black = rawdata - metadata.e_blackpoints;

	rawdata = (e_tmp_black.cwiseMax(0.0f).array() / metadata.e_scaling_gain.array()).matrix();


	Eigen::MatrixXf g_2 = metadata.e_dualiso_gain.cwiseProduct(metadata.e_dualiso_gain);
	variance = (g_2 * metadata.i_exposure_t * metadata.i_na).cwiseProduct(rawdata) + 
				metadata.e_sr.cwiseProduct(metadata.e_sr);

	variance = (variance.array() / (g_2 * pow(metadata.i_exposure_t,2) * pow(metadata.i_na,2)).array()).matrix(); 		

	weights = (1 / variance.array()).matrix();

	
	Eigen::MatrixXf e_tmp_sat = camera_rawdata - (metadata.i_saturation_point - metadata.e_blackpoints.array() ).matrix();
	

	
	for(int j=0; j < metadata.i_height; j++)
	{

		for (int i = 0; i < metadata.i_width; ++i)
		{
			// Clip values below blackpoint
			if(e_tmp_black(j,i) <=0)
				weights(j,i) = 0;
			// Clip values above saturation point
			//  only for higher ISO, set the weights to zero and variances to a high number

			if(e_tmp_sat(j,i) >=0 && metadata.e_scaling_gain(j,i) > 2.0f)
			{
				weights(j,i) = 0;
				variance(j,i) = pow(10, 30);

			}
			
		}

	}


	
	return true;

}

} // namespace vcl