#include "ofxNDIVideoCaster.h"
#include "ofBaseTypes.h"
#include "ofPixels.h"

using namespace std;
using namespace ofxNDI::Send;

void VideoCaster::setSource(std::shared_ptr<ofBaseVideo> source, bool async)
{
	source_ = source;
	stream_ = make_shared<ofxNDISendVideoAsync>();
	stream_->setup(*this);
}
void VideoCaster::update()
{
	if(!source_) return;
	if(source_->isFrameNew()) {
		ofPixels pix = source_->getPixels();
		pix.setImageType(OF_IMAGE_COLOR_ALPHA);
		stream_->send(pix);
	}
}

