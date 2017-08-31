//
//  ofxNDIAudioFrameDecoder.cpp
//  TestMac
//
//  Created by Iwatani Nariaki on 2017/04/07.
//
//

#include "ofxNDIAudioFrameDecoder.h"

using namespace ofxNDI;

template<> void AudioDecoder::freeFrame() {
	NDIlib_recv_free_audio_v2(receiver_, &frame_.back());
}
template<> bool AudioDecoder::captureFrame() {
	return NDIlib_recv_capture_v2(receiver_, nullptr, &frame_.back(), nullptr, timeout_ms_) == NDIlib_frame_type_audio;
}

template<> template<>
void AudioDecoder::decodeTo<ofSoundBuffer>(ofSoundBuffer &dst)
{
	if(is_front_allocated_) {
		auto &frame = frame_.front();
		dst.copyFrom(frame.p_data, frame.no_samples, frame.no_channels, frame.sample_rate);
	}
}
