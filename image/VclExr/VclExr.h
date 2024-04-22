#ifndef __VCL_EXR_H__
#define __VCL_EXR_H__

#include <string>

#include <opencv2/opencv.hpp>

//
// This class does not keep a copy of the image it is about to write.
// It only holds onto a reference to the image, and therefore does
// no memory cleanup of its own (aside from the header attributes).
// Usage and examples are at the bottom of this file
// 

//
// It should be noted that at the VCL, all OpenCV images are stored in RGB
// order (even though the image library insists everything is BGR.  If you
// load an image off disk using openCV calls directly, the following line
// should be included after loading to swap the two channels:
//
// cv::cvtColor(inputImage, inputImage, CV_BGR2RGB, 0);
//

//
// Example writing OpenExr with this library:
// The following code is a pretty common use-case.
// > Vcl::VclExr foo(testMatrix);
// > foo.addStringAttribute("VclFoo", "bar");
// > foo.writeExrGreyscaleHalf("/tmp/half.exr");
// > foo.writeExrGreyscaleFloat("/tmp/float.exr");
//
// Example reading OpenExr with this library:
// Greyscale images load single-channel, 32-bit, rgb images load 3-channel.
// > cv::Mat test0 = Vcl::VclExr::readExr("/tmp/float.exr");
// > Vcl::VclExr::readStringAttribute("/tmp/half.exr", "VclFoo");
//

namespace Vcl
{


class VclExr
{
public:
    VclExr(const cv::Mat& image);
    ~VclExr();

    // Attribute wrangling
    bool addStringAttribute(const std::string& attrName, const std::string& attrValue);
    bool addFloatAttribute(const std::string& attrName, const float& attrValue);
    
    // RGB writing
    bool writeExrRgbHalf(const std::string& filename);
    bool writeExrRgbFloat(const std::string& filename);
    
    // Greyscale writing
    bool writeExrGreyscaleHalf(const std::string& filename);
    bool writeExrGreyscaleFloat(const std::string& filename);
    
private:
    const cv::Mat& m_imageHandle;
    std::map<std::string, std::string> m_stringAttrMap;
    std::map<std::string, float> m_floatAttrMap;
    
public:
    static cv::Mat readExr(const std::string& filename);
    
    static bool readStringAttribute(const std::string& filename, const std::string& attributeName, std::string& value);

    static bool readFloatAttribute(const std::string& filename, const std::string& attributeName, float& value);
    static bool readFloatAttributes(const std::string& filename, const std::vector<std::string>& attributeNames, std::vector<float>& values);

    static bool setThreadCount(const int& threadCount);
};


}
#endif //__VCL_EXR_H__
