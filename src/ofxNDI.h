#pragma once

#include <Processing.NDI.structs.h>
#include "ofPixels.h"
#include "ofSoundBuffer.h"

namespace ofxNDI {

#pragma mark video
	static void decode(const NDIlib_video_frame_v2_t frame, ofPixels &dst) {
		ofPixelFormat format;
		switch(frame.FourCC) {
			case NDIlib_FourCC_type_RGBX:	format = OF_PIXELS_RGB;		break;
			case NDIlib_FourCC_type_BGRX:	format = OF_PIXELS_BGR;		break;
			case NDIlib_FourCC_type_RGBA:	format = OF_PIXELS_RGBA;	break;
			case NDIlib_FourCC_type_BGRA:	format = OF_PIXELS_BGRA;	break;
			case NDIlib_FourCC_type_UYVY:	format = OF_PIXELS_UYVY;	break;
			default:
				ofLogWarning("ofxNDI : this pixel format is not supported");
				break;
		}
		dst.setFromPixels(frame.p_data, frame.xres, frame.yres, format);
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

#pragma mark audio
	static void decode(const NDIlib_audio_frame_v2_t frame, ofSoundBuffer &dst) {
		dst.copyFrom(frame.p_data, frame.no_samples*sizeof(float)/frame.channel_stride_in_bytes, frame.no_channels, frame.sample_rate);
	}
	static NDIlib_audio_frame_v2_t encode(const ofSoundBuffer &src) {
		NDIlib_audio_frame_v2_t frame;
		frame.sample_rate = src.getSampleRate();
		frame.no_channels = src.getNumChannels();
		frame.no_samples = src.size();
		frame.channel_stride_in_bytes = src.size()/src.getNumFrames()*sizeof(float);
		frame.p_data = const_cast<float*>(src.getBuffer().data());
		return frame;
	}
}
