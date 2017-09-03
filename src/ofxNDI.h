#pragma once

#include <Processing.NDI.structs.h>
#include "ofPixels.h"
#include "ofSoundBuffer.h"

namespace ofxNDI {
	static void decode(const NDIlib_video_frame_v2_t frame, ofPixels &dst) {
		dst.setFromPixels(frame.p_data, frame.xres, frame.yres, frame.line_stride_in_bytes/frame.xres);
	}
	static NDIlib_video_frame_v2_t encode(const ofPixels &src) {
		NDIlib_video_frame_v2_t frame;
		int w = (int)src.getWidth(), h = (int)src.getHeight();
		frame.xres = w;
		frame.yres = h;
		frame.picture_aspect_ratio = w/(float)h;
		frame.line_stride_in_bytes = (int)(w*src.getBytesPerPixel());
		frame.p_data = const_cast<unsigned char*>(src.getData());
		switch(src.getPixelFormat()) {
			case OF_PIXELS_RGB:		frame.FourCC = NDIlib_FourCC_type_RGBX;	break;
			case OF_PIXELS_BGR:		frame.FourCC = NDIlib_FourCC_type_BGRX;	break;
			case OF_PIXELS_RGBA:	frame.FourCC = NDIlib_FourCC_type_RGBA;	break;
			case OF_PIXELS_BGRA:	frame.FourCC = NDIlib_FourCC_type_BGRA;	break;
			case OF_PIXELS_UYVY:	frame.FourCC = NDIlib_FourCC_type_UYVY;	break;
			default:
				ofLogWarning("ofxNDI : this pixel format is not supported");
				break;
		}
		return frame;
	}


	static void decode(const NDIlib_audio_frame_v2_t frame, ofSoundBuffer &dst) {
		dst.copyFrom(frame.p_data, frame.no_samples, frame.no_channels, frame.sample_rate);
	}
}
