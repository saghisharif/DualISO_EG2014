#ifndef CAMERA_H
#define CAMERA_H

#include <map>
#include <string>

#include "BlackArea.h"
#include "pugixml.hpp"
#include "ColorFilterArray.h"
#include "CameraSensorInfo.h"
#include "CameraMetadataException.h"


/* 
    RawSpeed - RAW file decoder.

    Copyright (C) 2009-2014 Klaus Post

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

    http://www.klauspost.com
*/

namespace RawSpeed {


class Camera
{
public:
  Camera(pugi::xml_node &camera);
  Camera(const Camera* camera, uint32 alias_num);
  void parseCameraChild( pugi::xml_node &node );
  const CameraSensorInfo* getSensorInfo(int iso);
  virtual ~Camera(void);
  std::string make;
  std::string model;
  std::string mode;
  std::string canonical_make;
  std::string canonical_model;
  std::string canonical_alias;
  std::string canonical_id;
  std::vector<std::string> aliases;
  std::vector<std::string> canonical_aliases;
  ColorFilterArray cfa;
  bool supported;
  iPoint2D cropSize;
  iPoint2D cropPos;
  std::vector<BlackArea> blackAreas;
  std::vector<CameraSensorInfo> sensorInfo;
  int decoderVersion;
  std::map<std::string,std::string> hints;
protected:
  void parseCFA( pugi::xml_node &node );
  void parseAlias( pugi::xml_node &node );
  void parseHint( pugi::xml_node &node );
  void parseID( pugi::xml_node &node );
  void parseBlackAreas( pugi::xml_node &node );
  void parseSensorInfo( pugi::xml_node &node );
  std::vector<int> MultipleStringToInt(const char *in, const char *tag, const char* attribute);
};

} // namespace RawSpeed

#endif
