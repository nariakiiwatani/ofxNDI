//
//  ofxNDIAudioFrameDecoder.h
//  TestMac
//
//  Created by Iwatani Nariaki on 2017/04/07.
//
//

#pragma once

#include "ofxNDIFrameDecoder.h"
#include "ofSoundBuffer.h"

namespace ofxNDI
{
	using AudioDecoder = FrameDecoder<NDIlib_audio_frame_v2_t>;
	template<> template<> void AudioDecoder::decodeTo<ofSoundBuffer>(ofSoundBuffer &dst);
}
