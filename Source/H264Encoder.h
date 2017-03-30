/*=========================================================================

  Program:   OpenIGTLink
  Language:  C++

  Copyright (c) Insight Software Consortium. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

/*!
 * \copy
 *     Copyright (c)  2013, Cisco Systems
 *     All rights reserved.
 *
 *     Redistribution and use in source and binary forms, with or without
 *     modification, are permitted provided that the following conditions
 *     are met:
 *
 *        * Redistributions of source code must retain the above copyright
 *          notice, this list of conditions and the following disclaimer.
 *
 *        * Redistributions in binary form must reproduce the above copyright
 *          notice, this list of conditions and the following disclaimer in
 *          the documentation and/or other materials provided with the
 *          distribution.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *     "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *     FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *     COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *     BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *     CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *     LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *     ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *     POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef __igtlH264Encoder_h
#define __igtlH264Encoder_h

#include <time.h>
#if defined(_WIN32) /*&& defined(_DEBUG)*/
  #include <windows.h>
  #include <stdio.h>
  #include <stdarg.h>
  #include <sys/types.h>
#else
  #include <sys/time.h>
#endif
#include <vector>

#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iostream>

#include "igtlCodecCommonClasses.h"
#include "sha1.h"
#include "igtl_header.h"
#include "igtl_video.h"
#include "igtlOSUtil.h"
#include "igtlMessageHeader.h"
#include "igtlVideoMessage.h"
#include "igtlTimeStamp.h"
#include "codec_def.h"
#include "codec_app_def.h"
#include "read_config.h"
#include "wels_const.h"


using namespace std;

typedef struct LayerpEncCtx_s {
  int32_t       iDLayerQp;
  SSliceArgument  sSliceArgument;
} SLayerPEncCtx;

class ISVCEncoder;

class H264Encoder: public GenericEncoder
{
public:
  H264Encoder(char * configFile = NULL);
  ~H264Encoder();
  
  void UpdateHashFromFrame (SFrameBSInfo& info, SHA1Context* ctx);
  
  bool CompareHash (const unsigned char* digest, const char* hashStr);

  virtual int FillSpecificParameters();
  
  /**
   Parse the configuration file to initialize the encoder and server.
   */
  virtual int InitializeEncoder();
  
  /**
   Encode a frame, for performance issue, before encode the frame, make sure the frame pointer is updated with a new frame.
   Otherwize, the old frame will be encoded.
   */
  virtual int EncodeSingleFrameIntoVideoMSG(SourcePicture* pSrcPic, igtl::VideoMessage* videoMessage, bool isGrayImage = false );
  
  virtual void SetPicWidth(unsigned int width);
  
  virtual void SetPicHeight(unsigned int height);
  
  virtual unsigned int GetPicWidth(){return this->sSvcParam.iPicWidth;};
  
  virtual unsigned int GetPicHeight(){return this->sSvcParam.iPicHeight;};
  
  virtual void SetLosslessLink(bool linkMethod);
  
  virtual bool GetLosslessLink(){return this->sSvcParam.bIsLosslessLink;};
  
private:
  int ParseLayerConfig (string strTag[], const int iLayer, SEncParamExt& pSvcParam);
  
  int ParseConfig();
  
  ISVCEncoder*  pSVCEncoder;
  
  SEncParamExt sSvcParam;
  
  std::string layerConfigFiles[MAX_DEPENDENCY_LAYER];
  // for configuration file
  
  SFrameBSInfo sFbi;
  
};

#endif
