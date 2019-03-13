#include "ofxNDISendStream.h"

#pragma mark video

void ofxNDISendVideo::sendFrame(const ofxNDI::VideoFrame &frame) const
{
	if(is_async_) {
		NDIlib_send_send_video_async_v2(instance_, &frame);
	}
	else {
		NDIlib_send_send_video_v2(instance_, &frame);
	}
}

template<>
void ofxNDISendAudio::sendFrame(const ofxNDI::AudioFrame &frame) const
{
	NDIlib_send_send_audio_v2(instance_, &frame);
}

void ofxNDISendMetadata::sendFrame(const ofxNDI::MetadataFrame &frame) const
{
	NDIlib_send_send_metadata(instance_, &frame);
}

