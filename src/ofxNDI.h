#pragma once

#include <stddef.h>
#include <Processing.NDI.Lib.h>
#include "ofPixels.h"
#include "ofSoundBuffer.h"

namespace ofxNDI {

	enum class Location {
		LOCAL,REMOTE,BOTH
	};
	struct Source {
		Source(){}
		Source(const NDIlib_source_t &src)
		:p_ndi_name(src.p_ndi_name)
		,p_url_address(src.p_url_address){}
		std::string p_ndi_name, p_url_address;
		operator NDIlib_source_t() const {
			return NDIlib_source_t(p_ndi_name.c_str(), p_url_address.c_str());
		}
	};
	static std::vector<Source> listSources(uint32_t waittime_ms=1000, Location location=Location::BOTH, const std::string &group="", const std::vector<std::string> extra_ips={}) {
		if (!NDIlib_initialize())
		{
			printf("Cannot run NDI.");
			return {};
		}
		
        auto getSourceInfo = [](uint32_t waittime_ms, bool local, const std::string &group, const std::vector<std::string> &extra_ips) -> std::vector<Source> {
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
			std::vector<Source> ret;
			ret.insert(std::end(ret), sources, sources+num_sources);
			for(auto &r : ret) {
				ofLogVerbose("ofxNDI::listSources") << "NDI Source Detected : " << r.p_ndi_name << "," << r.p_url_address;
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
				both.erase(std::remove_if(std::begin(both), std::end(both),[&remote](const Source &s){
					return std::find_if(std::begin(remote), std::end(remote), [&s](const Source &r){
						return s.p_ndi_name == r.p_ndi_name && s.p_url_address == r.p_url_address;
					}) != std::end(remote);
				}), std::end(both));
				return both;
			}
		}
		return {};
	}
}
