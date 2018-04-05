#pragma once

#include <Processing.NDI.structs.h>
#include "ofPixels.h"
#include "ofSoundBuffer.h"

#ifdef TARGET_WIN32
#include <Processing.NDI.deprecated.h>
#define OFXNDI_USE_V2
#endif
namespace ofxNDI {

	enum class Location {
		LOCAL,REMOTE,BOTH
	};
	static std::vector<NDIlib_source_t> listSources(uint32_t waittime_ms=1000, Location location=Location::BOTH, const std::string &group="", const std::vector<std::string> extra_ips={}) {
		if (!NDIlib_initialize())
		{
			printf("Cannot run NDI.");
			return {};
		}
		
		auto getSourceInfo = [](uint32_t waittime_ms, bool local, const string &group, const vector<string> extra_ips) -> vector<NDIlib_source_t> {
			const NDIlib_find_create_t settings = {local, group.c_str(), ofJoinString(extra_ips, ",").c_str()};
			NDIlib_find_instance_t finder = NDIlib_find_create_v2(&settings);
			if (!finder) return {};
			
			unsigned int num_sources=0;
			const NDIlib_source_t* sources;
			float endtime = ofGetElapsedTimef()+waittime_ms/1000.f;
			while(endtime > ofGetElapsedTimef()) {
				NDIlib_find_wait_for_sources(finder, 100);
			}
			
			sources = NDIlib_find_get_current_sources(finder, &num_sources);
			vector<NDIlib_source_t> ret;
			ret.insert(end(ret), sources, sources+num_sources);
			for(auto &r : ret) {
				cout << "NDI Source Detected : " << r.p_ndi_name << "," << r.p_url_address << endl;
			}
			NDIlib_find_destroy(finder);
			return ret;
		};
		
		switch(location) {
			case Location::BOTH:
				return getSourceInfo(waittime_ms, true, group, extra_ips);
			case Location::REMOTE:
				return getSourceInfo(waittime_ms, false, group, extra_ips);
			case Location::LOCAL: {
				auto&& both = getSourceInfo(waittime_ms, true, group, extra_ips);
				auto&& remote = getSourceInfo(waittime_ms, false, group, extra_ips);
				both.erase(remove_if(begin(both),end(both),[&remote](const NDIlib_source_t &s){
					return find_if(begin(remote), end(remote), [&s](const NDIlib_source_t &r){
						return strcmp(s.p_ndi_name,r.p_ndi_name)==0 && strcmp(s.p_url_address,r.p_url_address)==0;
					}) != end(remote);
				}), end(both));
				return both;
			}
		}
		return {};
	}

#pragma mark video
	static void decode(const NDIlib_video_frame_v2_t frame, ofPixels &dst) {
		ofPixelFormat format;
		switch(frame.FourCC) {
			case NDIlib_FourCC_type_RGBX:
			case NDIlib_FourCC_type_RGBA:	format = OF_PIXELS_RGBA;	break;
			case NDIlib_FourCC_type_BGRX:
			case NDIlib_FourCC_type_BGRA:	format = OF_PIXELS_BGRA;	break;
			case NDIlib_FourCC_type_UYVY:	format = OF_PIXELS_UYVY;	break;
			default:
				ofLogWarning("ofxNDI : this pixel format is not supported");
				break;
		}
		dst.setFromPixels(frame.p_data, frame.xres, frame.yres, format);
	}
	static NDIlib_video_frame_v2_t encode(ofPixels &&src) {
		switch(src.getPixelFormat()) {
			case OF_PIXELS_RGB:
			case OF_PIXELS_BGR:		src.setImageType(OF_IMAGE_COLOR_ALPHA);	break;
			default: break;
		}

		NDIlib_video_frame_v2_t frame;
		int w = (int)src.getWidth(), h = (int)src.getHeight();
		frame.xres = w;
		frame.yres = h;
		frame.picture_aspect_ratio = w/(float)h;
		frame.line_stride_in_bytes = (int)(w*src.getBytesPerPixel());
		frame.p_data = static_cast<unsigned char*>(src.getData());
		switch(src.getPixelFormat()) {
//			case OF_PIXELS_RGB:		frame.FourCC = NDIlib_FourCC_type_RGBX;	break;
//			case OF_PIXELS_BGR:		frame.FourCC = NDIlib_FourCC_type_BGRX;	break;
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
		dst.copyFrom(frame.p_data, frame.channel_stride_in_bytes/sizeof(decltype(*frame.p_data)), frame.no_channels, frame.sample_rate);
	}
	static NDIlib_audio_frame_v2_t encode(ofSoundBuffer &&src) {
		NDIlib_audio_frame_v2_t frame;
		frame.sample_rate = src.getSampleRate();
		frame.no_channels = src.getNumChannels();
		frame.no_samples = src.size();
		frame.p_data = static_cast<float*>(src.getBuffer().data());
		frame.channel_stride_in_bytes = src.getNumFrames()*sizeof(decltype(*frame.p_data));
		return frame;
	}

#pragma mark metadata
	static void decode(const NDIlib_metadata_frame_t frame, std::string &dst) {
		dst = frame.p_data;
	}
}
