#pragma once

#include "ofxNDIReceiver.h"

namespace ofxNDI {
namespace Recv {

class PTZControl
{
public:
	bool setup(Receiver &receiver);
	bool setup(Receiver::Instance instance);
	
	bool zoom(float zoom) const;
	bool zoomBySpeed(float speed) const;
	
	bool panTilt(float pan, float tilt) const;
	bool panTiltBySpeed(float pan_speed, float tilt_speed) const;
	
	bool storePreset(int index) const;
	bool recallPreset(int index, float speed) const;
	
	bool autoFocus() const;
	bool focus(float focus) const;
	bool focusBySpeed(float speed) const;
	
	enum WhiteBalanceMode {
		AUTO, INDOOR, OUTDOOR, ONESHOT
	};
	bool whiteBalanceMode(WhiteBalanceMode mode) const;
	bool whiteBalance(float red, float blue) const;
	
	bool exposureAuto() const;
	bool exposure(float exposure) const;
	bool exposure(float iris, float gain, float shutter_speed) const;
private:
	Receiver::Instance instance_;
};
}}

using ofxNDIPTZControl = ofxNDI::Recv::PTZControl;
