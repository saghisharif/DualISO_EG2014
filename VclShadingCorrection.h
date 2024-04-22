#ifndef __VCL_SHADING_CORRECTION_H__
#define __VCL_SHADING_CORRECTION_H__

/*-----------------------------------------------------------------------------* 
 * Reading the camera calibration data and computes an estimate of the incident 
 * radiant power from noisy digital input sample values 
 * For more information visit : http://vcl.itn.liu.se/publications/2014/HKU14/
 * Linkoping University
 *-----------------------------------------------------------------------------*/
#include <iostream>

#include <eigen3/Eigen/Dense>


namespace Vcl{

struct camera_metadata
{
	int i_nBits;
	int i_width;
	int i_height;
	int i_exposure_t;
	int i_G1Pattern[2];
	int i_BPattern[2];
	int i_RPattern[2];
	int i_G2Pattern[2];
	int i_na;
	int i_saturation_point;

	Eigen::MatrixXf e_scaling_gain;
	Eigen::MatrixXf e_dualiso_gain;
	Eigen::MatrixXf e_sr;
	Eigen::MatrixXf e_blackpoints;

	std::string s_camera_model;
};

class Vclshadingcorrection{

public:
	Vclshadingcorrection();
	~Vclshadingcorrection()
	{

	};
	bool read_calibration(std::string fname);
	bool read_raw_image(std::string fname, std::string xml_fname);
	bool estimate_radiant_pow();

	Eigen::MatrixXf rawdata, variance, weights;


private:

	camera_metadata metadata;
};



}


#endif // __VCL_SHADING_CORRECTION_H__