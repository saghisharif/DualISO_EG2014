#include "VclRaw.h"

// RawSpeed image decoding
#include <RawDecoder.h>
#include <FileReader.h>
#include <RawParser.h>

// Exif parsing
#include <exiv2/image.hpp>
#include <exiv2/easyaccess.hpp>


namespace Vcl
{


int vclRawSpeedThreadCount = 8;
void setRawSpeedThreadCount(const int& threads)
{ 
    vclRawSpeedThreadCount = threads; 
}


cv::Mat loadRawImage(const std::string& filename, const std::string& camerasFilename)
{
    RawSpeed::CameraMetaData meta(camerasFilename.c_str());
    
    RawSpeed::FileReader fReader((char*)filename.c_str());
    RawSpeed::FileMap* fMap = fReader.readFile();
    
    RawSpeed::RawParser parser(fMap);
    RawSpeed::RawDecoder* decoder = parser.getDecoder();
    decoder->checkSupport(&meta);
    decoder->decodeRaw();
    decoder->decodeMetaData(&meta);
    //decoder->mRaw->scaleBlackWhite();
    
    RawSpeed::RawImage rawImage = decoder->mRaw;
    
    if (rawImage->metadata.subsampling.x != 1 || rawImage->metadata.subsampling.y != 1)
        throw std::runtime_error("Subsampled RAW images are currently not supported.");
    if (rawImage->getDataType() != RawSpeed::TYPE_USHORT16)
        throw std::runtime_error("Only RAW data in 16-bit format is currently supported.");
    if (!rawImage->isCFA)
        throw std::runtime_error("Only sensors with a color filter array are currently supported.");

    const int width = rawImage->dim.x;
    const int height = rawImage->dim.y;
    const int pitch = rawImage->pitch / sizeof(uint16_t);
    const uint16_t* data = (uint16_t*)rawImage->getData(0, 0);
    
    cv::Mat cvImage(height, width, CV_16U, cv::Scalar(0.0f));
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            const uint16_t& value16  = data[y*pitch + x];
            cvImage.at<uint16_t>(y, x) = value16;
        }
    }
    
    delete decoder;
    delete fMap;
    
    return cvImage;
    
    // Just some notes to show that we can get black level and whitepoint if we want it
    // (and ISO speed, oddly enough)
    /*
    std::string mode;
    if (decoder->mRaw->isCFA)
      mode = "cfa";
    else if (decoder->mRaw->metadata.subsampling.y == 2)
      mode = "sRaw1";
    else
      mode = "sRaw2";
    
    printf("Camera: %s, %s, mode:%s\r\n",
           decoder->getRootIFD()->getEntryRecursive(RawSpeed::MAKE)->getString().c_str(), 
           decoder->getRootIFD()->getEntryRecursive(RawSpeed::MODEL)->getString().c_str(), 
           mode.c_str());
    
    printf("%s: <Sensor black=\"%d\", white=\"%d\" black_colors=\"%d,%d,%d,%d\" iso_list=\"%d\"/>\r\n",
           mode.c_str(),
           decoder->mRaw->blackLevel, 
           decoder->mRaw->whitePoint, 
           decoder->mRaw->blackLevelSeparate[0], 
           decoder->mRaw->blackLevelSeparate[1], 
           decoder->mRaw->blackLevelSeparate[2], 
           decoder->mRaw->blackLevelSeparate[3], 
           decoder->mRaw->metadata.isoSpeed);
    */
}


// Returns the black level for the given raw image (specified in cameras.xml)
int rawImageBlackLevel(const std::string& filename, const std::string& camerasFilename)
{
    RawSpeed::CameraMetaData meta(camerasFilename.c_str());
    
    RawSpeed::FileReader fReader((char*)filename.c_str());
    RawSpeed::FileMap* fMap = fReader.readFile();
    
    RawSpeed::RawParser parser(fMap);
    RawSpeed::RawDecoder* decoder = parser.getDecoder();
    decoder->checkSupport(&meta);
    decoder->decodeRaw();
    decoder->decodeMetaData(&meta);

    const int blackLevel = decoder->mRaw->blackLevel;
    delete decoder;
    delete fMap;
    
    return blackLevel;
}


// Returns the white point for the given raw image (specified in cameras.xml)
int rawImageWhitePoint(const std::string& filename, const std::string& camerasFilename)
{
    RawSpeed::CameraMetaData meta(camerasFilename.c_str());
    
    RawSpeed::FileReader fReader((char*)filename.c_str());
    RawSpeed::FileMap* fMap = fReader.readFile();
    
    RawSpeed::RawParser parser(fMap);
    RawSpeed::RawDecoder* decoder = parser.getDecoder();
    decoder->checkSupport(&meta);
    decoder->decodeRaw();
    decoder->decodeMetaData(&meta);

    const int whitePoint = decoder->mRaw->whitePoint;
    delete decoder;
    delete fMap;
    
    return whitePoint;
}


// Returns the exposure time for the given image
float rawImageExposureTime(const std::string& filename)
{
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
    if (image.get() == 0)
        throw std::runtime_error("\"" + filename + "\": could not open RAW file!");
    image->readMetadata();
    
    Exiv2::ExifData::const_iterator it;
    const Exiv2::ExifData& exifData = image->exifData();
    
    it = Exiv2::exposureTime(exifData);
    if (it == exifData.end())
        throw std::runtime_error("\"" + filename + "\": could not extract the exposure time!");
    return it->toFloat();
}


// Returns the iso speed for the given image
float rawImageIso(const std::string& filename)
{
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
    if (image.get() == 0)
        throw std::runtime_error("\"" + filename + "\": could not open RAW file!");
    image->readMetadata();
    
    Exiv2::ExifData::const_iterator it;
    const Exiv2::ExifData& exifData = image->exifData();

    it = Exiv2::isoSpeed(exifData);
    if (it == exifData.end())
        throw std::runtime_error("\"" + filename + "\": could not extract the ISO speed!");
    return it->toFloat();
}


// Returns the aperture for the given image
float rawImageAperture(const std::string& filename)
{
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
    if (image.get() == 0)
        throw std::runtime_error("\"" + filename + "\": could not open RAW file!");
    image->readMetadata();
    
    Exiv2::ExifData::const_iterator it;
    const Exiv2::ExifData& exifData = image->exifData();

    it = Exiv2::fNumber(exifData);
    if (it == exifData.end())
        throw std::runtime_error("\"" + filename + "\": could not extract the aperture setting!");
    return it->toFloat();
}


// Returns the exposure mode for the given image
std::string rawImageExposureMode(const std::string& filename)
{
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
    if (image.get() == 0)
        throw std::runtime_error("\"" + filename + "\": could not open RAW file!");
    image->readMetadata();
    
    Exiv2::ExifData::const_iterator it;
    const Exiv2::ExifData& exifData = image->exifData();

    it = Exiv2::exposureMode(exifData);
    if (it == exifData.end())
        throw std::runtime_error("\"" + filename + "\": could not extract the exposure mode!");
    return it->print(&exifData);
}


// Returns the focus mode for the given image
std::string rawImageFocusMode(const std::string& filename)
{
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
    if (image.get() == 0)
        throw std::runtime_error("\"" + filename + "\": could not open RAW file!");
    image->readMetadata();
    
    Exiv2::ExifData::const_iterator it;
    const Exiv2::ExifData& exifData = image->exifData();

    it = exifData.findKey(Exiv2::ExifKey("Exif.CanonCs.FocusMode"));
    if (it == exifData.end())
        return "unknown";

    return it->print(&exifData);
}


}


// Rawspeed has a dumb pull model for retrieving the number of cores it can use
// Give it something to work with here
int rawspeed_get_number_of_processor_cores()
{
    return Vcl::vclRawSpeedThreadCount;
}


