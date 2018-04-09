#pragma once

#include "ofxiOS.h"
#include "ofxNDISender.h"
#include "ofxNDISendStream.h"

class ofApp : public ofxiOSApp{
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void touchDown(ofTouchEventArgs & touch);
	void touchMoved(ofTouchEventArgs & touch);
	void touchUp(ofTouchEventArgs & touch);
	void touchDoubleTap(ofTouchEventArgs & touch);
	void touchCancelled(ofTouchEventArgs & touch);
	
	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	void deviceOrientationChanged(int newOrientation);
private:
	ofxNDISender sender_;
	ofxNDISendVideo video_;
	ofVideoGrabber camera_;
};
