//
//  ofxNDIPTZ.cpp
//  example-sender-macos
//
//  Created by Iwatani Nariaki on 2018/04/05.
//
//

#include "ofxNDIPTZ.h"

bool ofxNDIPTZControl::setup(ofxNDIReceiver &receiver)
{
	return setup(receiver.getInstance());
}
bool ofxNDIPTZControl::setup(ofxNDIReceiver::Instance instance)
{
	instance_ = instance;
	return NDIlib_recv_ptz_is_supported(instance_);
}

bool ofxNDIPTZControl::zoom(float zoom) const
{
	return NDIlib_recv_ptz_zoom(instance_, zoom);
}
bool ofxNDIPTZControl::zoomBySpeed(float speed) const
{
	return NDIlib_recv_ptz_zoom_speed(instance_, speed);
}

bool ofxNDIPTZControl::panTilt(float pan, float tilt) const
{
	return NDIlib_recv_ptz_pan_tilt(instance_, pan, tilt);
}
bool ofxNDIPTZControl::panTiltBySpeed(float pan_speed, float tilt_speed) const
{
	return NDIlib_recv_ptz_pan_tilt_speed(instance_, pan_speed, tilt_speed);
}

bool ofxNDIPTZControl::storePreset(int index) const
{
	return NDIlib_recv_ptz_store_preset(instance_, index);
}
bool ofxNDIPTZControl::recallPreset(int index, float speed) const
{
	return NDIlib_recv_ptz_recall_preset(instance_, index, speed);
}

bool ofxNDIPTZControl::autoFocus() const
{
	return NDIlib_recv_ptz_auto_focus(instance_);
}
bool ofxNDIPTZControl::focus(float focus) const
{
	return NDIlib_recv_ptz_focus(instance_, focus);
}
bool ofxNDIPTZControl::focusBySpeed(float speed) const
{
	return NDIlib_recv_ptz_focus_speed(instance_, speed);
}

bool ofxNDIPTZControl::whiteBalanceMode(WhiteBalanceMode mode) const
{
	switch(mode) {
		case AUTO: return NDIlib_recv_ptz_white_balance_auto(instance_);
		case INDOOR: return NDIlib_recv_ptz_white_balance_indoor(instance_);
		case OUTDOOR: return NDIlib_recv_ptz_white_balance_outdoor(instance_);
		case ONESHOT: return NDIlib_recv_ptz_white_balance_oneshot(instance_);
	}
	return false;
}
bool ofxNDIPTZControl::whiteBalance(float red, float blue) const
{
	return NDIlib_recv_ptz_white_balance_manual(instance_, red, blue);
}

bool ofxNDIPTZControl::exposureAuto() const
{
	return NDIlib_recv_ptz_exposure_auto(instance_);
}
bool ofxNDIPTZControl::exposure(float exposure) const
{
	return NDIlib_recv_ptz_exposure_manual(instance_, exposure);
}
bool ofxNDIPTZControl::exposure(float iris, float gain, float shutter_speed) const
{
	return NDIlib_recv_ptz_exposure_manual_v2(instance_, iris, gain, shutter_speed);
}
