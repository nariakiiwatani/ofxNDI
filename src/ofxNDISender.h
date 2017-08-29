#pragma once

#include "ofConstants.h"
#include <Processing.NDI.Lib.h>
#include <string>
#include "ofPixels.h"

class ofxNDISender
{
public:
	ofxNDISender();
	~ofxNDISender();
	
	void setup(const std::string &name, const std::string &group="", bool clock_video=false, bool clock_audio=false);
	void addConnectionMetadata(const std::string &metadata, int64_t timecode=NDIlib_send_timecode_synthesize);
	void clearConnectionMetadata();
	
	bool isSetup() const { return sender_!=nullptr; }
	
	
	void setFrameRate(int rate_n, int rate_d) { video_frame_.frame_rate_N=rate_n, video_frame_.frame_rate_D=rate_d; }
	void setFourCCType(NDIlib_FourCC_type_e four_cc) { video_frame_.FourCC = four_cc; }
	void setFrameFormatType(NDIlib_frame_format_type_e format) { video_frame_.frame_format_type = format; }
	void setAsync(bool async) { is_async_ = async; }
	
	void sendVideoFrame(const ofPixels &frame) {
		video_frame_.xres = (int)frame.getWidth();
		video_frame_.yres = (int)frame.getHeight();
		video_frame_.picture_aspect_ratio = video_frame_.xres/(float)video_frame_.yres;
		video_frame_.line_stride_in_bytes = (int)(video_frame_.xres*frame.getBytesPerPixel());
		video_frame_.p_data = const_cast<unsigned char*>(frame.getData());
		if(is_async_) {
			NDIlib_send_send_video_async_v2(sender_, &video_frame_);
		}
		else {
			NDIlib_send_send_video_v2(sender_, &video_frame_);
		}
	}
private:
	NDIlib_send_instance_t sender_=nullptr;
	NDIlib_video_frame_v2_t video_frame_;
	bool is_async_ = false;
};
