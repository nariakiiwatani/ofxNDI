#pragma once

#include <stddef.h>
#include <Processing.NDI.Advanced.h>
#include <string>
#include <vector>
#include <mutex>
#include <thread>

namespace ofxNDI {

struct Source {
	std::string ndi_name, url_address, metadata;
	Source(const char *p_ndi_name=nullptr, const char *p_url_address=nullptr, const char *p_metadata=nullptr) {
		ndi_name = p_ndi_name ? p_ndi_name : "";
		url_address = p_url_address ? p_url_address : "";
		metadata = p_metadata ? p_metadata : "";
	}
	Source(NDIlib_source_t v):Source(v.p_ndi_name, v.p_url_address){}
	Source(NDIlib_source_v2_t v):Source(v.p_ndi_name, v.p_url_address, v.p_metadata){}
	NDIlib_source_t toV1() const { return {ndi_name.c_str(), url_address.c_str()}; }
	NDIlib_source_v2_t toV2() const { return {ndi_name.c_str(), url_address.c_str(), ""}; }
};

namespace Find {
class Finder
{
public:
	~Finder();
	std::function<std::vector<Source>(bool, bool)> watchSources(bool show_local_resources=true, const std::string &group="", const std::vector<std::string> extra_ips={});
	std::vector<Source> getSources() const;
	void terminate(bool wait);

	std::vector<Source> listSources(std::size_t waittime_ms=1000, bool show_local_resources=true, const std::string &group="", const std::vector<std::string> extra_ips={});

	void setFilter(const std::string &name_or_url);
private:
	mutable std::mutex mutex_;
	std::thread watcher_;
	std::vector<Source> found_sources_;
	bool terminate_;
	struct Filter {
		std::string name_or_url;
		bool isMatch(const Source &source) const;
	} filter_;
	
	std::vector<Source> filter(const std::vector<Source> &source) const;
};

}}

using ofxNDIFinder = ofxNDI::Find::Finder;
