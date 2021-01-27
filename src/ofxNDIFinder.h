#pragma once

#include <stddef.h>
#include <Processing.NDI.Lib.h>

#include <string>
#include <vector>
#include <mutex>
#include <thread>

namespace ofxNDI {
class Finder
{
public:
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

}

using ofxNDIFinder = ofxNDI::Finder;
