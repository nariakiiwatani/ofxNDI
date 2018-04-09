#pragma once

#include <Processing.NDI.Lib.h>
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
		
		auto getSourceInfo = [](uint32_t waittime_ms, bool local, const string &group, const vector<string> extra_ips) -> vector<Source> {
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
			vector<Source> ret;
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
				both.erase(remove_if(begin(both),end(both),[&remote](const Source &s){
					return find_if(begin(remote), end(remote), [&s](const Source &r){
						return s.p_ndi_name == r.p_ndi_name && s.p_url_address == r.p_url_address;
					}) != end(remote);
				}), end(both));
				return both;
			}
		}
		return {};
	}
}
