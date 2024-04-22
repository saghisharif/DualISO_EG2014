#include "VclExr.h"

#include <OpenEXR/ImfInputFile.h>
#include <OpenEXR/ImfOutputFile.h>
#include <OpenEXR/ImfChannelList.h>
#include <OpenEXR/ImfStringAttribute.h>
#include <OpenEXR/ImfFloatAttribute.h>

namespace Vcl
{


VclExr::VclExr(const cv::Mat& image) 
    : m_imageHandle(image)
    , m_stringAttrMap()
    , m_floatAttrMap()
{ }


VclExr::~VclExr()
{ }


bool VclExr::addStringAttribute(const std::string& attrName, const std::string& attrValue)
{
    std::pair<std::map<std::string, std::string>::iterator, bool> ret;
    ret = m_stringAttrMap.insert(std::pair<std::string, std::string>(attrName, attrValue));
    return ret.second;
}


bool VclExr::addFloatAttribute(const std::string& attrName, const float& attrValue)
{
    std::pair<std::map<std::string, float>::iterator, bool> ret;
    ret = m_floatAttrMap.insert(std::pair<std::string, float>(attrName, attrValue));
    return ret.second;
}


bool VclExr::writeExrRgbHalf(const std::string& filename)
{
    // TODO: Much of this code is shared with writeExrRgbFloat()
    //       A clever way to reduce this duplication would be grand.
    if (m_imageHandle.channels() != 3 && m_imageHandle.channels() != 4)
    {
        std::cerr << "Attempting to write RGB image from " << m_imageHandle.channels() << "-channel OpenCV matrix." << std::endl;
        return false;
    }

    if (m_imageHandle.depth() != CV_32F)
    {
        std::cerr << "Attempting to write floating-point image from OpenCV matrix that does not contain floats." << std::endl;
        return false;
    }

    // Convert the floats to halfs
    const int& width = m_imageHandle.cols;
    const int& height = m_imageHandle.rows;

    // Can be optimized with code from:
    // https://lists.nongnu.org/archive/html/openexr-devel/2011-07/msg00000.html
    std::vector<half> halfImage;
    halfImage.reserve(width*height*3);
    float *currentImage = (float*)m_imageHandle.data;
    for(int row = 0; row < height*3; row++)
    {
        for(int col = 0; col < width; col++)
        {
            const float& v = currentImage[(width*row) + col];
            halfImage.push_back(half(v));
        }
    }
    
    // Write the exr
    Imf::Header header(width, height);
    header.channels().insert ("R", Imf::Channel(Imf::HALF));
    header.channels().insert ("G", Imf::Channel(Imf::HALF));
    header.channels().insert ("B", Imf::Channel(Imf::HALF));
    
    for (std::map<std::string, std::string>::iterator it = m_stringAttrMap.begin(); it != m_stringAttrMap.end(); ++it)
    {
        header.insert(it->first, Imf::StringAttribute(it->second));
    }
    for (std::map<std::string, float>::iterator it = m_floatAttrMap.begin(); it != m_floatAttrMap.end(); ++it)
    {
        header.insert(it->first, Imf::FloatAttribute(it->second));
    }
    
    Imf::FrameBuffer frameBuffer;
    frameBuffer.insert("R",
                       Imf::Slice(Imf::HALF,
                                  (char*)(&halfImage[0]),
                                  sizeof(half) * 3,
                                  sizeof(half) * width * 3));
    frameBuffer.insert("G",
                       Imf::Slice(Imf::HALF,
                                  (char*)(&halfImage[1]),
                                  sizeof(half) * 3,
                                  sizeof(half) * width * 3));
    frameBuffer.insert("B",
                       Imf::Slice(Imf::HALF,
                                  (char*)(&halfImage[2]),
                                  sizeof(half) * 3,
                                  sizeof(half) * width * 3));

    Imf::OutputFile file(filename.c_str(), header);
    file.setFrameBuffer(frameBuffer);
    file.writePixels(m_imageHandle.rows);
    return true;
}


bool VclExr::writeExrRgbFloat(const std::string& filename)
{
    if (m_imageHandle.channels() != 3 && m_imageHandle.channels() != 4)
    {
        std::cerr << "Attempting to write RGB image from " << m_imageHandle.channels() << "-channel OpenCV matrix." << std::endl;
        return false;
    }

    if (m_imageHandle.depth() != CV_32F)
    {
        std::cerr << "Attempting to write floating-point image from OpenCV matrix that does not contain floats." << std::endl;
        return false;
    }

    // Write the exr
    const int& width = m_imageHandle.cols;
    const int& height = m_imageHandle.rows;

    Imf::Header header(width, height);
    header.channels().insert ("R", Imf::Channel(Imf::FLOAT));
    header.channels().insert ("G", Imf::Channel(Imf::FLOAT));
    header.channels().insert ("B", Imf::Channel(Imf::FLOAT));
    
    for (std::map<std::string, std::string>::iterator it = m_stringAttrMap.begin(); it != m_stringAttrMap.end(); ++it)
    {
        header.insert(it->first, Imf::StringAttribute(it->second));
    }
    for (std::map<std::string, float>::iterator it = m_floatAttrMap.begin(); it != m_floatAttrMap.end(); ++it)
    {
        header.insert(it->first, Imf::FloatAttribute(it->second));
    }
    
    Imf::FrameBuffer frameBuffer;
    frameBuffer.insert("R",
                       Imf::Slice(Imf::FLOAT,
                                  (char*)(&m_imageHandle.data[0*sizeof(float)]),
                                  sizeof(float) * 3,
                                  sizeof(float) * width * 3));
    frameBuffer.insert("G",
                       Imf::Slice(Imf::FLOAT,
                                  (char*)(&m_imageHandle.data[1*sizeof(float)]),
                                  sizeof(float) * 3,
                                  sizeof(float) * width * 3));
    frameBuffer.insert("B",
                       Imf::Slice(Imf::FLOAT,
                                  (char*)(&m_imageHandle.data[2*sizeof(float)]),
                                  sizeof(float) * 3,
                                  sizeof(float) * width * 3));

    Imf::OutputFile file(filename.c_str(), header);
    file.setFrameBuffer(frameBuffer);
    file.writePixels(m_imageHandle.rows);
    return true;
}


bool VclExr::writeExrGreyscaleHalf(const std::string& filename)
{
    // TODO: Much of this code is shared with writeExrGreyscaleFloat()
    //       A clever way to reduce this duplication would be grand.
    if (m_imageHandle.channels() != 1)
    {
        std::cerr << "Attempting to write greyscale image from multi-channel OpenCV matrix." << std::endl;
        return false;
    }

    if (m_imageHandle.depth() != CV_32F)
    {
        std::cerr << "Attempting to write floating-point image from OpenCV matrix that does not contain floats." << std::endl;
        return false;
    }

    // Convert the floats to halfs
    const int& width = m_imageHandle.cols;
    const int& height = m_imageHandle.rows;

    // Can be optimized with code from:
    // https://lists.nongnu.org/archive/html/openexr-devel/2011-07/msg00000.html
    std::vector<half> halfImage;
    halfImage.reserve(width*height);
    float *currentImage = (float*)m_imageHandle.data;
    for(int row = 0; row < height; row++)
    {
        for(int col = 0; col < width; col++)
        {
            const float& g = currentImage[(width*row) + col];
            halfImage.push_back(half(g));
        }
    }
    
    
    // Write the exr
    Imf::Header header(width, height);
    header.channels().insert ("G", Imf::Channel(Imf::HALF));
    
    for (std::map<std::string, std::string>::iterator it = m_stringAttrMap.begin(); it != m_stringAttrMap.end(); ++it)
    {
        header.insert(it->first, Imf::StringAttribute(it->second));
    }
    for (std::map<std::string, float>::iterator it = m_floatAttrMap.begin(); it != m_floatAttrMap.end(); ++it)
    {
        header.insert(it->first, Imf::FloatAttribute(it->second));
    }
    
    Imf::FrameBuffer frameBuffer;
    frameBuffer.insert("G",
                       Imf::Slice(Imf::HALF,
                                  (char*)(&halfImage[0]),
                                  sizeof(half),
                                  sizeof(half) * width));

    Imf::OutputFile file(filename.c_str(), header);
    file.setFrameBuffer(frameBuffer);
    file.writePixels(height);
    return true;
}


bool VclExr::writeExrGreyscaleFloat(const std::string& filename)
{
    if (m_imageHandle.channels() != 1)
    {
        std::cerr << "Attempting to write greyscale image from multi-channel OpenCV matrix." << std::endl;
        return false;
    }

    if (m_imageHandle.depth() != CV_32F)
    {
        std::cerr << "Attempting to write floating-point image from OpenCV matrix that does not contain floats." << std::endl;
        return false;
    }

    // Write the exr    
    const int& width = m_imageHandle.cols;
    const int& height = m_imageHandle.rows;
    
    Imf::Header header(width, height);
    header.channels().insert ("G", Imf::Channel(Imf::FLOAT));
    
    for (std::map<std::string, std::string>::iterator it = m_stringAttrMap.begin(); it != m_stringAttrMap.end(); ++it)
    {
        header.insert(it->first, Imf::StringAttribute(it->second));
    }
    for (std::map<std::string, float>::iterator it = m_floatAttrMap.begin(); it != m_floatAttrMap.end(); ++it)
    {
        header.insert(it->first, Imf::FloatAttribute(it->second));
    }
    
    Imf::FrameBuffer frameBuffer;
    frameBuffer.insert("G",
                       Imf::Slice(Imf::FLOAT,
                                  (char*)m_imageHandle.data,
                                  sizeof(float),
                                  sizeof(float) * width));

    Imf::OutputFile file(filename.c_str(), header);
    file.setFrameBuffer(frameBuffer);
    file.writePixels(height);
    return true;
}


// Reads an exr into a cv::Mat data structure.
// Properly creates a single-channel cv::Mat if the input file is greyscale
cv::Mat VclExr::readExr(const std::string& filename)
{
    // Detect if we're greyscale or RGB
    int channelCount = 0;
    Imf::InputFile testFile(filename.c_str());
    const Imf::ChannelList &channels = testFile.header().channels();
    for (Imf::ChannelList::ConstIterator i = channels.begin(); i != channels.end(); ++i)
    {
        channelCount++;
    }

    // Downgrade to a single-channel image (with data assumed to be in the green channel) if we've only got one channel
    cv::Mat inputImage = cv::imread(filename.c_str(), CV_LOAD_IMAGE_COLOR|CV_LOAD_IMAGE_ANYDEPTH);
    if (channelCount == 1)
    {
        cv::Mat singleChannelImage(inputImage.rows, inputImage.cols, CV_32F);
        for (int i = 0; i < inputImage.rows * inputImage.cols; i++)
        {
            float& Mi = ((float*)singleChannelImage.data)[i];
            Mi = ((float*)inputImage.data)[i*3+1];
        }
        return singleChannelImage;
    }
    else
    {
        // This actually converts from on-disk RGB to OpenCV BGR
        cv::cvtColor(inputImage, inputImage, CV_BGR2RGB, 0);
    }
    return inputImage;
}


bool VclExr::readStringAttribute(const std::string& filename, const std::string& attributeName, std::string& value)
{
    Imf::InputFile headerReadFile(filename.c_str());
    const Imf::StringAttribute* attr = headerReadFile.header().findTypedAttribute<Imf::StringAttribute>(attributeName.c_str());
    if(attr == NULL)
        return false;
    value = attr->value();
    return true;
}


bool VclExr::readFloatAttribute(const std::string& filename, const std::string& attributeName, float& value)
{
    Imf::InputFile headerReadFile(filename.c_str());
    const Imf::FloatAttribute* attr = headerReadFile.header().findTypedAttribute<Imf::FloatAttribute>(attributeName.c_str());
    if(attr == NULL)
        return false;
    value = attr->value();
    return true;
}


bool VclExr::readFloatAttributes(const std::string& filename, const std::vector<std::string>& attributeNames, std::vector<float>& values)
{
    Imf::InputFile headerReadFile(filename.c_str());
    for (int i = 0; i < attributeNames.size(); i++)
    {
        const Imf::FloatAttribute* attr = headerReadFile.header().findTypedAttribute<Imf::FloatAttribute>(attributeNames[i].c_str());
        if(attr == NULL)
            return false;
        values.push_back(attr->value());
    }
    return true;
}


bool VclExr::setThreadCount(const int &threadCount)
{
    Imf::setGlobalThreadCount(threadCount);
    return true;
}


}
