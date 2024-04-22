#ifndef RAF_DECODER_H
#define RAF_DECODER_H

#include "RawDecoder.h"
#include "TiffIFD.h"
#include "BitPumpPlain.h"
#include "TiffParser.h"
/*
    RawSpeed - RAW file decoder.

    Copyright (C) 2013 Klaus Post
    Copyright (C) 2014 Pedro Côrte-Real

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

class RafDecoder :
  public RawDecoder
{
public:
  RafDecoder(TiffIFD *rootIFD, FileMap* file);
  virtual ~RafDecoder(void);
  RawImage decodeRawInternal();
  virtual void decodeMetaDataInternal(CameraMetaData *meta);
  virtual void checkSupportInternal(CameraMetaData *meta);
  TiffIFD *mRootIFD;
  virtual TiffIFD* getRootIFD() {return mRootIFD;}
protected:
  virtual void decodeThreaded(RawDecoderThread* t);
  void DecodeRaf();
  bool alt_layout;
};

} // namespace RawSpeed

#endif
