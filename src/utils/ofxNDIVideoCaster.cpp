#include "ofxNDIVideoCaster.h"
#include "ofBaseTypes.h"

using namespace std;
using namespace ofxNDI::Send;

void VideoCaster::setSource(std::shared_ptr<ofBaseVideo> source, bool async)
{
	source_ = source;
	stream_ = make_shared<ofxNDISendVideo>();
	stream_->setup(*this);
	stream_->setAsync(async);
}
void VideoCaster::update()
{
	if(!source_) return;
	if(source_->isFrameNew()) {
		stream_->send(source_->getPixels());
	}
}

