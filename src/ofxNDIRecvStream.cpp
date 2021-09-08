#include "ofxNDIRecvStream.h"
#include <Processing.NDI.AVSync.h>

#pragma mark Video Stream

template<>
int ofxNDI::Recv::Stream<ofxNDI::VideoFrame>::getQueue() const
{
	NDIlib_recv_queue_t frames;
	NDIlib_recv_get_queue(instance_, &frames);
	return frames.video_frames;
}
template<>
int64_t ofxNDI::Recv::Stream<ofxNDI::VideoFrame>::getNumReceivedFrame() const
{
	NDIlib_recv_performance_t frames;
	NDIlib_recv_get_performance(instance_, &frames, nullptr);
	return frames.video_frames;
}
template<>
int64_t ofxNDI::Recv::Stream<ofxNDI::VideoFrame>::getNumDroppedFrame() const
{
	NDIlib_recv_performance_t frames;
	NDIlib_recv_get_performance(instance_, nullptr, &frames);
	return frames.video_frames;
}
template<> template<>
inline void ofxNDI::Recv::Stream<ofxNDI::VideoFrame>::setAllocator(void* p_opaque, NDIlib_video_alloc_t p_allocator, NDIlib_video_free_t p_deallocator) {
	NDIlib_recv_set_video_allocator(instance_, p_opaque, p_allocator, p_deallocator);
}

template<>
void ofxNDI::Recv::Stream<ofxNDI::VideoFrame>::freeFrame(ofxNDI::VideoFrame &frame) {
	NDIlib_recv_free_video_v2(instance_, &frame);
}
template<>
bool ofxNDI::Recv::Stream<ofxNDI::VideoFrame>::captureFrame(ofxNDI::VideoFrame &frame) {
	return NDIlib_recv_capture_v2(instance_, &frame, nullptr, nullptr, timeout_ms_) == NDIlib_frame_type_video;
}
bool ofxNDIRecvVideoFrameSync::captureFrame(ofxNDI::VideoFrame &frame) {
	NDIlib_framesync_capture_video(sync_, &frame, field_type_);
	return frame.p_data != nullptr;
}
void ofxNDIRecvVideoFrameSync::freeFrame(ofxNDI::VideoFrame &frame) {
	NDIlib_framesync_free_video(sync_, &frame);
}

#pragma mark Audio Stream

template<>
int ofxNDI::Recv::Stream<ofxNDI::AudioFrame>::getQueue() const
{
	NDIlib_recv_queue_t frames;
	NDIlib_recv_get_queue(instance_, &frames);
	return frames.audio_frames;
}
template<>
int64_t ofxNDI::Recv::Stream<ofxNDI::AudioFrame>::getNumReceivedFrame() const
{
	NDIlib_recv_performance_t frames;
	NDIlib_recv_get_performance(instance_, &frames, nullptr);
	return frames.audio_frames;
}
template<>
int64_t ofxNDI::Recv::Stream<ofxNDI::AudioFrame>::getNumDroppedFrame() const
{
	NDIlib_recv_performance_t frames;
	NDIlib_recv_get_performance(instance_, nullptr, &frames);
	return frames.audio_frames;
}
template<> template<>
inline void ofxNDI::Recv::Stream<ofxNDI::AudioFrame>::setAllocator(void* p_opaque, NDIlib_audio_alloc_t p_allocator, NDIlib_audio_free_t p_deallocator) {
	NDIlib_recv_set_audio_allocator(instance_, p_opaque, p_allocator, p_deallocator);
}

template<>
void ofxNDI::Recv::Stream<ofxNDI::AudioFrame>::freeFrame(ofxNDI::AudioFrame &frame) {
	NDIlib_recv_free_audio_v3(instance_, &frame);
}
template<>
bool ofxNDI::Recv::Stream<ofxNDI::AudioFrame>::captureFrame(ofxNDI::AudioFrame &frame) {
	return NDIlib_recv_capture_v3(instance_, nullptr, &frame, nullptr, timeout_ms_) == NDIlib_frame_type_audio;
}
bool ofxNDIRecvAudioFrameSync::captureFrame(ofxNDI::AudioFrame &frame) {
	NDIlib_framesync_capture_audio_v2(sync_, &frame, sample_rate_, num_channels_, num_samples_);
	return frame.no_channels != 0;
}
void ofxNDIRecvAudioFrameSync::freeFrame(ofxNDI::AudioFrame &frame) {
	NDIlib_framesync_free_audio_v2(sync_, &frame);
}
int ofxNDIRecvAudioFrameSync::getNumQueuedSamples() const {
	return NDIlib_framesync_audio_queue_depth(sync_);
}


#pragma mark Metadata Stream

template<>
int ofxNDI::Recv::Stream<ofxNDI::MetadataFrame>::getQueue() const
{
	NDIlib_recv_queue_t frames;
	NDIlib_recv_get_queue(instance_, &frames);
	return frames.metadata_frames;
}
template<>
int64_t ofxNDI::Recv::Stream<ofxNDI::MetadataFrame>::getNumReceivedFrame() const
{
	NDIlib_recv_performance_t frames;
	NDIlib_recv_get_performance(instance_, &frames, nullptr);
	return frames.metadata_frames;
}
template<>
int64_t ofxNDI::Recv::Stream<ofxNDI::MetadataFrame>::getNumDroppedFrame() const
{
	NDIlib_recv_performance_t frames;
	NDIlib_recv_get_performance(instance_, nullptr, &frames);
	return frames.metadata_frames;
}
template<>
bool ofxNDI::Recv::Stream<ofxNDI::MetadataFrame>::captureFrame(ofxNDI::MetadataFrame &frame) {
	return NDIlib_recv_capture_v2(instance_, nullptr, nullptr, &frame, timeout_ms_) == NDIlib_frame_type_metadata;
}
template<>
void ofxNDI::Recv::Stream<ofxNDI::MetadataFrame>::freeFrame(ofxNDI::MetadataFrame &frame) {
	NDIlib_recv_free_metadata(instance_, &frame);
}

#pragma mark AVSync

ofxNDIRecvAVSyncAudio::~AVSyncAudio()
{
	unsubscribe_();
	if(av_sync_) {
		NDIlib_avsync_destroy(av_sync_);
	}
}
void ofxNDIRecvAVSyncAudio::setup(ofxNDIReceiver &receiver)
{
	Threading::setup(receiver);
	av_sync_ = NDIlib_avsync_create(receiver.getInstance());
}
void ofxNDIRecvAVSyncAudio::setupSourceStream(Stream<SourceFrame> &source)
{
	unsubscribe_();
	ofAddListener(source.onFrameReceived, this, &ofxNDIRecvAVSyncAudio::onSourceFrameReceived);
	unsubscribe_ = [&]() {
		ofRemoveListener(source.onFrameReceived, this, &ofxNDIRecvAVSyncAudio::onSourceFrameReceived);
	};
}
void ofxNDIRecvAVSyncAudio::onSourceFrameReceived(const SourceFrame &source)
{
	std::lock_guard<std::mutex> lock(source_mtx_);
	source_frame_ = &source;
}

bool ofxNDIRecvAVSyncAudio::captureFrame(ofxNDI::AudioFrame &frame)
{
	std::lock_guard<std::mutex> lock(source_mtx_);
	return NDIlib_avsync_synchronize(av_sync_, source_frame_, &frame_.back()) >= NDIlib_avsync_ret_success;
}
void ofxNDIRecvAVSyncAudio::freeFrame(ofxNDI::AudioFrame &frame)
{
	NDIlib_avsync_free_audio(av_sync_, &frame);
}
