#pragma once

#include <stddef.h>
#include <Processing.NDI.Lib.h>
#include "ofxNDIFrame.h"
#include "ofxNDIReceiver.h"
#include "ofxNDIRecvStrategy.h"
#include "ofxNDI.h"

namespace ofxNDI {
namespace Recv {

template<typename Frame, typename Wrapper=ofxNDIReceiver>
class Stream
{
public:
	template<typename Strategy>
	std::shared_ptr<Strategy> setup(Wrapper &wrapper) {
		strategy_ = std::make_shared<Strategy>(wrapper);
		return std::static_pointer_cast<Strategy>(strategy_);
	}
	void update() {
		strategy_->update();
		is_frame_new_ = strategy_->isFrameNew();
	}
	template<typename Output> void decodeTo(Output &dst) const {
		strategy_->getFrame().decode(dst);
	}
	bool isFrameNew() const { return is_frame_new_; }
	std::string getMetadata() const { 
		static_assert(!std::is_same<Frame, ofxNDI::MetadataFrame>::value, "this function is not for ofxNDIRecvMetadata");
		return strategy_->getFrame().p_metadata;
	}
protected:
	std::shared_ptr<FrameSyncStrategy<Frame, Wrapper>> strategy_;
	bool is_frame_new_=false;
};
}}
using ofxNDIRecvVideo = ofxNDI::Recv::Stream<ofxNDI::VideoFrame>;
using ofxNDIRecvAudio = ofxNDI::Recv::Stream<ofxNDI::AudioFrame>;
using ofxNDIRecvMetadata = ofxNDI::Recv::Stream<ofxNDI::MetadataFrame>;
