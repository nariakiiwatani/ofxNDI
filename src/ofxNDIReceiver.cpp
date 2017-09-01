#include "ofxNDIReceiver.h"
#include <Processing.NDI.Lib.h>
#include "ofUtils.h"

using namespace std;

vector<ofxNDIReceiver::Source> ofxNDIReceiver::listSources(uint32_t waittime_ms, Location location, const string &group, const vector<string> extra_ips)
{
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
		ret.reserve(num_sources);
		for(;num_sources-->0;) {
			ret.emplace_back(*sources);
			cout << "NDI Source Detected : " << sources->p_ndi_name << "," << sources->p_ip_address << endl;
			++sources;
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
					return strcmp(s.p_ndi_name,r.p_ndi_name)==0 && strcmp(s.p_ip_address,r.p_ip_address)==0;
				}) != end(remote);
			}), end(both));
			return both;
		}
	}
	return {};
}

bool ofxNDIReceiver::setup(size_t index, const Settings &settings)
{
	auto &&sources = listSources();
	if(index < sources.size()) {
		return setup(sources[index], settings);
	}
	ofLogWarning("no NDI Source found");
	return false;
}
bool ofxNDIReceiver::setup(const Source &source, const Settings &settings)
{
	NDIlib_recv_create_t creator = { source, settings.color_format, settings.bandwidth, settings.deinterlace };
	receiver_ = NDIlib_recv_create_v2(&creator);
	if (!receiver_) {
		ofLogError("NDI Receiver failed to initialize");
		return false;
	}
	return true;
}

bool ofxNDIReceiver::isConnected() const
{
	return receiver_!=nullptr && NDIlib_recv_get_no_connections(receiver_);
}

ofxNDIReceiver::~Receiver()
{
	NDIlib_recv_destroy(receiver_);
	NDIlib_destroy();
}

