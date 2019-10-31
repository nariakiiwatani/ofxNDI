#pragma once

#include "ofxNDISender.h"
#include "ofxNDISendStream.h"
#include <memory>

class ofBaseVideo;

namespace ofxNDI {
namespace Send {
class VideoCaster : public Sender
{
public:
	void setSource(std::shared_ptr<ofBaseVideo> source, bool async=true);
	void update();
private:
	std::shared_ptr<ofBaseVideo> source_;
	std::shared_ptr<ofxNDISendVideo> stream_;
};
}}

using ofxNDIVideoCaster = ofxNDI::Send::VideoCaster;
