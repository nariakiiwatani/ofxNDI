#include "ofxNDIVideoGrabber.h"
#include "ofxNDIRecvStream.h"

using namespace std;
using namespace ofxNDI;
using namespace ofxNDI::Recv;

namespace {
class BaseGrabber : public ofBaseVideoGrabber
{
public:
	BaseGrabber(Receiver &receiver):receiver_(receiver){}
	~BaseGrabber(){}
	
	std::vector<ofVideoDevice> listDevices() const {
		std::vector<ofVideoDevice> ret;
		auto sources = ofxNDI::listSources();
		for(auto &&s : sources) {
			ofVideoDevice v;
			v.id = ret.size();
			v.deviceName = s.p_ndi_name;
			v.hardwareName = s.p_url_address;
			v.bAvailable = true;
			ret.emplace_back(v);
		}
		return ret;
	}
	
	bool setup(int w, int h) {
		stream_ = make_shared<ofxNDIRecvVideoFrameSync>();
		stream_->setup(receiver_);
		return true;
	}
	void update() {
		if(stream_) {
			stream_->update();
			if(isFrameNew()) {
				stream_->decodeTo(pixels_);
			}
		}
	}
	bool isFrameNew() const {
		return stream_ ? stream_->isFrameNew() : false;
	}
	void close() {
		stream_.reset();
		pixels_.clear();
	}
	
	float getWidth() const { return pixels_.getWidth(); }
	float getHeight() const { return pixels_.getHeight(); }
	
	bool isInitialized() const { return (bool)stream_; }
    bool setPixelFormat(ofPixelFormat pixelFormat) { return false;  }
	ofPixelFormat getPixelFormat() const { return pixels_.getPixelFormat(); }
	ofPixels& getPixels() { return pixels_; }
	const ofPixels& getPixels() const { return pixels_; }
private:
	Receiver &receiver_;
	int device_id_ = 0;
	
	shared_ptr<ofxNDIRecvVideoFrameSync> stream_;
	ofPixels pixels_;
};
}

VideoGrabber::VideoGrabber()
{
	setGrabber(make_shared<BaseGrabber>(*this));
}

bool VideoGrabber::setup(std::size_t index, const Settings &settings)
{
	if(ofVideoGrabber::isInitialized()) {
		ofVideoGrabber::close();
	}
	if(Receiver::isSetup()) {
		Receiver::changeConnection(ofxNDI::listSources()[index]);
	}
	else {
		Receiver::setup(index, settings);
	}
	return Receiver::isSetup() && ofVideoGrabber::setup(0,0);
}
bool VideoGrabber::setup(const Source &source, const Settings &settings)
{
	if(ofVideoGrabber::isInitialized()) {
		ofVideoGrabber::close();
	}
	if(Receiver::isSetup()) {
		Receiver::changeConnection(source);
	}
	else {
		Receiver::setup(source, settings);
	}
	return Receiver::isSetup() && ofVideoGrabber::setup(0,0);
}
