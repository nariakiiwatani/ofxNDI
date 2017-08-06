//
//  ofxNDIFrameDecoders.h
//  TestMac
//
//  Created by Iwatani Nariaki on 2017/04/07.
//
//

#pragma once

#include "ofThread.h"
#include "DoubleBuffer.h"
#include <Processing.NDI.Lib.h>

namespace ofxNDI
{
	template<typename Frame>
	class FrameDecoder : private ofThread
	{
	public:
		void setup(NDIlib_recv_instance_t receiver, std::size_t timeout_ms, bool threaded=true);
		void update();
		bool isFrameNew() const { return is_frame_new_; }
		template<typename Output> void decodeTo(Output &output);
	private:
		NDIlib_recv_instance_t receiver_;
		DoubleBuffer<Frame> frame_;
		std::size_t timeout_ms_;
		bool is_frame_new_=false;
		bool has_new_frame_=false;
		bool is_front_allocated_=false;
		
		void threadedFunction();
		void updateFrame();
		bool captureFrame();
		void freeFrame();
	};
}

template<typename Frame>
void ofxNDI::FrameDecoder<Frame>::setup(NDIlib_recv_instance_t receiver, std::size_t timeout_ms, bool threaded) {
	receiver_ = receiver;
	timeout_ms_ = timeout_ms;
	if(threaded) {
		startThread();
	}
}
template<typename Frame>
void ofxNDI::FrameDecoder<Frame>::update() {
	if(!isThreadRunning()) {
		updateFrame();
	}
	is_frame_new_ = has_new_frame_;
	has_new_frame_ = false;
}

template<typename Frame>
void ofxNDI::FrameDecoder<Frame>::threadedFunction() {
	while(isThreadRunning()) {
		if(lock()) {
			updateFrame();
			unlock();
		}
	}
}
template<typename Frame>
void ofxNDI::FrameDecoder<Frame>::updateFrame() {
	if(captureFrame()) {
		frame_.swap();
		if(is_front_allocated_) {
			freeFrame();
		}
		is_front_allocated_ = true;
		has_new_frame_ = true;
	}
}
