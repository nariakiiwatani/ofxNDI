#pragma once

#include <stddef.h>
#include <Processing.NDI.Advanced.h>
#include <string>
#include <vector>
#include <mutex>
#include <thread>

namespace ofxNDI {

using Source = NDIlib_source_v2_t;
static NDIlib_source_t toV1(const NDIlib_source_v2_t &v2) { return {v2.p_ndi_name, v2.p_url_address}; }
static NDIlib_source_v2_t toV2(const NDIlib_source_t &v1) { return {v1.p_ndi_name, v1.p_url_address, ""}; }

namespace Find {
class Finder
{
public:
	~Finder();
	std::function<std::vector<Source>(bool, bool)> watchSources(bool show_local_resources=true, const std::string &group="", const std::vector<std::string> extra_ips={});
	std::vector<Source> getSources() const;
	void terminate(bool wait);

	std::vector<Source> listSources(std::size_t waittime_ms=1000, bool show_local_resources=true, const std::string &group="", const std::vector<std::string> extra_ips={});

private:
	mutable std::mutex mutex_;
	std::thread watcher_;
	std::vector<Source> found_sources_;
	bool terminate_;
};

}}

using ofxNDIFinder = ofxNDI::Find::Finder;
