#pragma once

#include "ofThread.h"
#include "DoubleBuffer.h"
#include <Processing.NDI.Lib.h>
#include "ofxNDIReceiver.h"
#include "ofxNDI.h"

namespace ofxNDI {
namespace Recv {

template<typename Frame>
class Stream : private ofThread
{
public:
	void setup(Receiver &receiver, uint32_t timeout_ms, bool threaded=true);
	void setup(NDIlib_recv_instance_t receiver, uint32_t timeout_ms, bool threaded=true);
	void update();
	bool isFrameNew() const { return is_frame_new_; }
	template<typename Output> void decodeTo(Output &dst);
	Frame& getFrame() const { return frame_.front(); }
private:
	NDIlib_recv_instance_t receiver_;
	DoubleBuffer<Frame> frame_;
	uint32_t timeout_ms_;
	bool is_frame_new_=false;
	bool has_new_frame_=false;
	bool is_front_allocated_=false;
	
	void threadedFunction();
	void updateFrame();
	bool captureFrame();
	void freeFrame();
};

template<typename Frame>
void Stream<Frame>::setup(Receiver &receiver, uint32_t timeout_ms, bool threaded) {
	setup(receiver.getReceiver(), timeout_ms, threaded);
}
template<typename Frame>
void Stream<Frame>::setup(NDIlib_recv_instance_t receiver, uint32_t timeout_ms, bool threaded) {
	receiver_ = receiver;
	timeout_ms_ = timeout_ms;
	if(threaded) {
		startThread();
	}
}
template<typename Frame>
void Stream<Frame>::update() {
	if(!isThreadRunning()) {
		updateFrame();
	}
	is_frame_new_ = has_new_frame_;
	has_new_frame_ = false;
}

template<typename Frame>
void Stream<Frame>::threadedFunction() {
	while(isThreadRunning()) {
		if(lock()) {
			updateFrame();
			unlock();
		}
	}
}
template<typename Frame>
void Stream<Frame>::updateFrame() {
	if(captureFrame()) {
		frame_.swap();
		if(is_front_allocated_) {
			freeFrame();
		}
		is_front_allocated_ = true;
		has_new_frame_ = true;
	}
}
template<typename Frame> template<typename Output>
void Stream<Frame>::decodeTo(Output &dst) {
	if(is_front_allocated_) {
		ofxNDI::decode(frame_.front(), dst);
	}
}
}}

using ofxNDIVideoRecvStream = ofxNDI::Recv::Stream<NDIlib_video_frame_v2_t>;
using ofxNDIAudioRecvStream = ofxNDI::Recv::Stream<NDIlib_audio_frame_v2_t>;
using ofxNDIMetadataRecvStream = ofxNDI::Recv::Stream<NDIlib_metadata_frame_t>;
