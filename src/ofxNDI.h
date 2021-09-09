#pragma once

#include "ofxNDIFinder.h"

namespace ofxNDI {
	static std::vector<Source> listSources(uint32_t waittime_ms=1000, bool show_local_sources=true, const std::string &group="", const std::vector<std::string> extra_ips={}) {
		ofxNDIFinder finder;
		return finder.listSources(waittime_ms, show_local_sources, group, extra_ips);
	}
}
