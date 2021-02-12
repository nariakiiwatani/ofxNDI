#include "ofxNDIFinder.h"
#include "ofUtils.h"
#include "ofLog.h"

using namespace ofxNDI;
using namespace std;


Finder::~Finder()
{
	terminate(true);
}

std::function<std::vector<Source>(bool, bool)> Finder::watchSources(bool show_local_resources, const std::string &group, const std::vector<std::string> extra_ips)
{
	if (!NDIlib_initialize()) {
		printf("Cannot run NDI.");
		return {};
	}
	
	terminate(true);
	terminate_ = false;
	found_sources_.clear();
	
	watcher_ = std::thread([&](NDIlib_find_create_t settings) {
		NDIlib_find_instance_t finder = NDIlib_find_create_v2(&settings);
		if (!finder) return;
		while(!terminate_) {
			if(!NDIlib_find_wait_for_sources(finder, 5000)) {
				continue;
			}
			unsigned int num_sources=0;
			const NDIlib_source_t* sources = NDIlib_find_get_current_sources(finder, &num_sources);
			std::vector<Source> ret;
			ret.insert(std::end(ret), sources, sources+num_sources);
			for(auto &r : ret) {
				ofLogVerbose("ofxNDI::listSources") << "NDI Source Detected : " << r.p_ndi_name << "," << r.p_url_address;
			}
			std::lock_guard<std::mutex> lock(mutex_);
			found_sources_ = ret;
		}
		NDIlib_find_destroy(finder);
	}, NDIlib_find_create_t{show_local_resources, group.c_str(), ofJoinString(extra_ips, ",").c_str()});
	
	return [&](bool terminate, bool wait) {
		if(terminate) {
			this->terminate(wait);
		}
		std::lock_guard<std::mutex> lock(mutex_);
		auto ret = found_sources_;
		return std::move(ret);
	};
}

void Finder::terminate(bool wait)
{
	terminate_ = true;
	if(watcher_.joinable()) {
		wait ? watcher_.join() : watcher_.detach();
	}
}

std::vector<Source> Finder::getSources() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	auto ret = found_sources_;
	return std::move(ret);
}

std::vector<Source> Finder::listSources(std::size_t waittime_ms, bool show_local_resources, const std::string &group, const std::vector<std::string> extra_ips)
{
	auto op = watchSources(show_local_resources, group, extra_ips);
	ofSleepMillis(waittime_ms);
	return op(true, false);
}
