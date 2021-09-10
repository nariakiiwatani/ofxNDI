#include "ofxNDISendStream.h"
#include "ofSoundBuffer.h"
#include "ofxNDIFrameCompressed.h"

using namespace ofxNDI;
using namespace ofxNDI::Send;

#pragma mark - Video

template<>
void Stream<VideoFrame>::sendFrame(const VideoFrame &frame) const {
	NDIlib_send_send_video_v2(instance_, &frame);
}
void VideoStreamAsync::sendFrame(const VideoFrame &frame) const {
	NDIlib_send_send_video_async_v2(instance_, &frame);
}
template<>
void Stream<VideoFrame>::sendScatter(const VideoFrame &frame, const NDIlib_frame_scatter_t *scatter) const {
	NDIlib_send_send_video_scatter(instance_, &frame, scatter);
}
void VideoStreamAsync::sendScatter(const VideoFrame &frame, const NDIlib_frame_scatter_t *scatter) const {
	NDIlib_send_send_video_scatter_async(instance_, &frame, scatter);
}
template<> template<>
void Stream<VideoFrame>::send(const CompressedVideoInfo &src, const std::string &metadata, int64_t timecode) {
	auto frame = createFrame(src);
	frame.p_metadata = metadata.c_str();
	frame.timecode = timecode;
	beforeSend(frame);
	sendScatter(frame, (const NDIlib_frame_scatter_t*)frame.p_data);
	data_buffer_.swap();
}


#pragma mark - Audio

template<> template<>
AudioFrameInterleaved Stream<AudioFrameInterleaved>::createFrame<ofSoundBuffer>(const ofSoundBuffer &src) {
	return Frame::encode<ofSoundBuffer, AudioFrameInterleaved>(src, (void*)src.getBuffer().data(), sizeof(float)*src.size());
}

template<>
void Stream<AudioFrame>::sendFrame(const AudioFrame &frame) const {
	NDIlib_send_send_audio_v3(instance_, &frame);
}
template<>
void Stream<AudioFrameInterleaved>::sendFrame(const AudioFrameInterleaved &frame) const {
	NDIlib_util_send_send_audio_interleaved_32f(instance_, &frame);
}

template<>
void Stream<AudioFrame>::sendScatter(const AudioFrame &frame, const NDIlib_frame_scatter_t *scatter) const {
	NDIlib_send_send_audio_scatter(instance_, &frame, scatter);
}
template<> template<>
void Stream<AudioFrame>::send(const CompressedAudioInfo &src, const std::string &metadata, int64_t timecode) {
	auto frame = createFrame(src);
	frame.p_metadata = metadata.c_str();
	frame.timecode = timecode;
	beforeSend(frame);
	sendScatter(frame, (const NDIlib_frame_scatter_t*)frame.p_data);
	data_buffer_.swap();
}


#pragma mark - Metadata

template<>
void Stream<MetadataFrame>::sendFrame(const MetadataFrame &frame) const {
	NDIlib_send_send_metadata(instance_, &frame);
}

template<>
void ofxNDI::Recv::Stream<ofxNDI::MetadataFrame, ofxNDISender>::freeFrame(ofxNDI::MetadataFrame &frame) {
	NDIlib_send_free_metadata(instance_, &frame);
}
template<>
bool ofxNDI::Recv::Stream<ofxNDI::MetadataFrame, ofxNDISender>::captureFrame(ofxNDI::MetadataFrame &frame) {
	return NDIlib_send_capture(instance_, &frame, timeout_ms_) == NDIlib_frame_type_metadata;
}


