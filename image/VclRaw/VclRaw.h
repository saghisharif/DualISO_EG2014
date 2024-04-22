#ifndef __VCL_RAW_H__
#define __VCL_RAW_H__

//
// Horrible, horrible stuff 
//
// OpenCV must be included before RawSpeed since RawSpeed
// does a bunch of typedefs that conflict with OpenCV in a very unsafe way.
//
#include <opencv2/opencv.hpp>


namespace Vcl
{


// The RawSpeed library allows you to set the number of threads it uses to load raw files
void setRawSpeedThreadCount(const int& threads);

// Loads a 16-bit unsigned bayer-pattern image from a path to the raw file on disk
// and a suitable cameras.xml file that describes what the camera is capable of for raw images.
cv::Mat loadRawImage(const std::string& filename, const std::string& camerasFilename);

// Returns the black level for the given raw image (specified in cameras.xml)
int rawImageBlackLevel(const std::string& filename, const std::string& camerasFilename);

// Returns the white point for the given raw image (specified in cameras.xml)
int rawImageWhitePoint(const std::string& filename, const std::string& camerasFilename);

// Returns the exposure time for the given image
float rawImageExposureTime(const std::string& filename);

// Returns the iso speed for the given image
float rawImageIso(const std::string& filename);

// Returns the aperture for the given image
float rawImageAperture(const std::string& filename);

// Returns the exposure mode for the given image
std::string rawImageExposureMode(const std::string& filename);

// Returns the focus mode for the given image
std::string rawImageFocusMode(const std::string& filename);


}
#endif //__VCL_RAW_H__
