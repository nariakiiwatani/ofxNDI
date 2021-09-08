#include "ofxNDISendStream.h"
#include "ofSoundBuffer.h"

using namespace ofxNDI;
using namespace ofxNDI::Send;

template<> template<>
AudioFrameInterleaved Stream<AudioFrameInterleaved>::createFrame<ofSoundBuffer>(const ofSoundBuffer &src) {
	return Frame::encode<ofSoundBuffer, AudioFrameInterleaved>(src, (void*)src.getBuffer().data(), sizeof(float)*src.size());
}

template<>
void Stream<VideoFrame>::sendFrame(const VideoFrame &frame) const
{
	NDIlib_send_send_video_v2(instance_, &frame);
}
void VideoStreamAsync::sendFrame(const VideoFrame &frame) const
{
	NDIlib_send_send_video_async_v2(instance_, &frame);
}
template<>
void Stream<AudioFrame>::sendFrame(const AudioFrame &frame) const
{
	NDIlib_send_send_audio_v3(instance_, &frame);
}
template<>
void Stream<AudioFrameInterleaved>::sendFrame(const AudioFrameInterleaved &frame) const
{
	NDIlib_util_send_send_audio_interleaved_32f(instance_, &frame);
}
template<>
void ofxNDI::Recv::Stream<ofxNDI::MetadataFrame, ofxNDISender>::freeFrame(ofxNDI::MetadataFrame &frame) {
	NDIlib_send_free_metadata(instance_, &frame);
}
template<>
bool ofxNDI::Recv::Stream<ofxNDI::MetadataFrame, ofxNDISender>::captureFrame(ofxNDI::MetadataFrame &frame) {
	return NDIlib_send_capture(instance_, &frame, timeout_ms_) == NDIlib_frame_type_metadata;
}


