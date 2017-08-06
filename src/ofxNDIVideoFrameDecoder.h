//
//  ofxNDIVideoFrameDecoder.h
//  TestMac
//
//  Created by Iwatani Nariaki on 2017/04/07.
//
//

#pragma once

#include "ofxNDIFrameDecoder.h"
#include "ofPixels.h"
#include "ofFbo.h"

namespace ofxNDI
{
	using VideoDecoder = FrameDecoder<NDIlib_video_frame_t>;
	template<> template<> void VideoDecoder::decodeTo<ofPixels>(ofPixels &dst);
}
