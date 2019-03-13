#pragma once

#include "ofThread.h"
#include "DoubleBuffer.h"
#include "ofxNDIReceiver.h"
#include "ofxNDISender.h"

namespace ofxNDI {
namespace Recv {
template<typename Frame, typename Wrapper=ofxNDIReceiver>
class FrameSyncStrategy
{
public:
	FrameSyncStrategy(Wrapper &wrapper) {
		instance_ = wrapper.getInstance();
	}
	virtual void update()=0;
	virtual bool isFrameNew() const=0;
	virtual Frame& getFrame()=0;
	virtual bool captureFrame(Frame &frame)=0;
	virtual void freeFrame(Frame &frame)=0;
protected:
	typename Wrapper::Instance instance_;
};

template<typename Frame, typename Wrapper=ofxNDIReceiver>
class Blocking : public FrameSyncStrategy<Frame, Wrapper>
{
public:
	using FrameSyncStrategy<Frame,Wrapper>::FrameSyncStrategy;
	void setTimeout(uint32_t milliseconds) { timeout_ms_ = milliseconds; }
	void update() {
		Frame frame;
		if(captureFrame(frame)) {
			swap(frame, frame_);
			freeFrame(frame);
			using ::std::swap;
			is_frame_new_ = true;
		}
		else {
			is_frame_new_ = false;
		}
	}
	Frame& getFrame() { return frame_; }
	bool isFrameNew() const { return is_frame_new_; }
private:
	bool captureFrame(Frame &frame);
	void freeFrame(Frame &frame);
	Frame frame_;
	bool is_frame_new_=false;
	uint32_t timeout_ms_=1000;
};
template<typename Frame, typename Wrapper=ofxNDIReceiver>
class Threading : public FrameSyncStrategy<Frame, Wrapper>, private ofThread
{
public:
	Threading(Wrapper &wrapper)
	:FrameSyncStrategy<Frame,Wrapper>(wrapper) {
		startThread();
	}
	~Threading() {
		if(isThreadRunning()) {
			waitForThread();
		}
	}
	void setTimeout(uint32_t milliseconds) { timeout_ms_ = milliseconds; }
	void update() {
		std::lock_guard<std::mutex> lock(mutex_);
		is_frame_new_ = has_new_frame_;
		has_new_frame_ = false;
	}
	
	void threadedFunction() {
		while(isThreadRunning()) {
			updateFrame();
			sleep(1);
		}
	}
	void updateFrame() {
		if(captureFrame()) {
			std::lock_guard<std::mutex> lock(mutex_);
			frame_.swap();
			freeFrame();
			has_new_frame_ = true;
		}
	}
	Frame& getFrame() { return frame_.front(); }
	bool isFrameNew() const { return is_frame_new_; }
private:
	bool captureFrame(Frame &frame);
	void freeFrame(Frame &frame);
	
	mutable std::mutex mutex_;
	DoubleBuffer<Frame> frame_;
	uint32_t timeout_ms_=1000;
	bool is_frame_new_=false;
	bool has_new_frame_=false;
};
template<typename Frame, typename Wrapper=ofxNDIReceiver>
class FrameSync : public FrameSyncStrategy<Frame, Wrapper>
{
	static_assert(!std::is_same<Frame, ofxNDI::MetadataFrame>::value, "FrameSync is not available for metadata");
public:
	FrameSync(Wrapper &wrapper) {
		
	}
};

#pragma mark Video Stream

template<> inline void Blocking<ofxNDI::VideoFrame, ofxNDIReceiver>::freeFrame(ofxNDI::VideoFrame &frame) {
	NDIlib_recv_free_video_v2(instance_, &frame);
}
template<> inline bool Blocking<ofxNDI::VideoFrame, ofxNDIReceiver>::captureFrame(ofxNDI::VideoFrame &frame) {
	return NDIlib_recv_capture_v2(instance_, &frame, nullptr, nullptr, timeout_ms_) == NDIlib_frame_type_video;
}
template<> inline void Threading<ofxNDI::VideoFrame, ofxNDIReceiver>::freeFrame(ofxNDI::VideoFrame &frame) {
	NDIlib_recv_free_video_v2(instance_, &frame);
}
template<> inline bool Threading<ofxNDI::VideoFrame, ofxNDIReceiver>::captureFrame(ofxNDI::VideoFrame &frame) {
	return NDIlib_recv_capture_v2(instance_, &frame, nullptr, nullptr, timeout_ms_) == NDIlib_frame_type_video;
}

#pragma mark Audio Stream
template<> inline void Blocking<ofxNDI::AudioFrame, ofxNDIReceiver>::freeFrame(ofxNDI::AudioFrame &frame) {
	NDIlib_recv_free_audio_v2(instance_, &frame);
}
template<> inline bool Blocking<ofxNDI::AudioFrame, ofxNDIReceiver>::captureFrame(ofxNDI::AudioFrame &frame) {
	return NDIlib_recv_capture_v2(instance_, nullptr, &frame, nullptr, timeout_ms_) == NDIlib_frame_type_audio;
}
template<> inline void Threading<ofxNDI::AudioFrame, ofxNDIReceiver>::freeFrame(ofxNDI::AudioFrame &frame) {
	NDIlib_recv_free_audio_v2(instance_, &frame);
}
template<> inline bool Threading<ofxNDI::AudioFrame, ofxNDIReceiver>::captureFrame(ofxNDI::AudioFrame &frame) {
	return NDIlib_recv_capture_v2(instance_, nullptr, &frame, nullptr, timeout_ms_) == NDIlib_frame_type_audio;
}

#pragma mark Metadata Stream

template<> inline void Blocking<ofxNDI::MetadataFrame, ofxNDIReceiver>::freeFrame(ofxNDI::MetadataFrame &frame) {
	NDIlib_recv_free_metadata(instance_, &frame);
}
template<> inline bool Blocking<ofxNDI::MetadataFrame, ofxNDIReceiver>::captureFrame(ofxNDI::MetadataFrame &frame) {
	return NDIlib_recv_capture_v2(instance_, nullptr, nullptr, &frame, timeout_ms_) == NDIlib_frame_type_metadata;
}
template<> inline void Threading<ofxNDI::MetadataFrame, ofxNDIReceiver>::freeFrame(ofxNDI::MetadataFrame &frame) {
	NDIlib_recv_free_metadata(instance_, &frame);
}
template<> inline bool Threading<ofxNDI::MetadataFrame, ofxNDIReceiver>::captureFrame(ofxNDI::MetadataFrame &frame) {
	return NDIlib_recv_capture_v2(instance_, nullptr, nullptr, &frame, timeout_ms_) == NDIlib_frame_type_metadata;
}
template<> inline void Blocking<ofxNDI::MetadataFrame, ofxNDISender>::freeFrame(ofxNDI::MetadataFrame &frame) {
	NDIlib_send_free_metadata(instance_, &frame);
}
template<> inline bool Blocking<ofxNDI::MetadataFrame, ofxNDISender>::captureFrame(ofxNDI::MetadataFrame &frame) {
	return NDIlib_send_capture(instance_, &frame, timeout_ms_) == NDIlib_frame_type_metadata;
}
template<> inline void Threading<ofxNDI::MetadataFrame, ofxNDISender>::freeFrame(ofxNDI::MetadataFrame &frame) {
	NDIlib_send_free_metadata(instance_, &frame);
}
template<> inline bool Threading<ofxNDI::MetadataFrame, ofxNDISender>::captureFrame(ofxNDI::MetadataFrame &frame) {
	return NDIlib_send_capture(instance_, &frame, timeout_ms_) == NDIlib_frame_type_metadata;
}

}}


