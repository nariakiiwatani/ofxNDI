#include "ofxNDIReceiver.h"
#include "ofUtils.h"

using namespace std;

vector<NDIlib_source_t> ofxNDIReceiver::listSources(uint32_t waittime_ms, Location location, const string &group, const vector<string> extra_ips)
{
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

bool ofxNDIReceiver::setup(size_t index, const Settings &settings)
{
	auto &&sources = listSources();
	if(index < sources.size()) {
		return setup(sources[index], settings);
	}
	ofLogWarning("no NDI Source found");
	return false;
}
bool ofxNDIReceiver::setup(const NDIlib_source_t &source, const Settings &settings)
{
	NDIlib_recv_create_v3_t creator = { source, settings.color_format, settings.bandwidth, settings.deinterlace };
	creator.p_ndi_name = settings.name==""?nullptr:settings.name.c_str();
	instance_ = NDIlib_recv_create_v3(&creator);
	if (!instance_) {
		ofLogError("NDI Receiver failed to initialize");
		return false;
	}
	return true;
}

void ofxNDIReceiver::addConnectionMetadata(const string &metadata, int64_t timecode)
{
	const NDIlib_metadata_frame_t data = {
		static_cast<int>(metadata.length()+1),
		timecode,
		const_cast<char*>(metadata.c_str())
	};
	NDIlib_recv_add_connection_metadata(instance_, &data);
}
void ofxNDIReceiver::clearConnectionMetadata()
{
	NDIlib_recv_clear_connection_metadata(instance_);
}

bool ofxNDIReceiver::isConnected() const
{
	return isSetup() && NDIlib_recv_get_no_connections(instance_);
}

void ofxNDIReceiver::getQueue(int &video, int &audio, int &metadata) const
{
	NDIlib_recv_queue_t que;
	NDIlib_recv_get_queue(instance_, &que);
	video = que.video_frames;
	audio = que.audio_frames;
	metadata = que.metadata_frames;
}

std::string ofxNDIReceiver::getWebControl() const
{
	const char *ptr = NDIlib_recv_get_web_control(instance_);
	std::string ret = ptr;
	NDIlib_recv_free_string(instance_, ptr);
	return ret;
}
ofxNDIReceiver::~Receiver()
{
	NDIlib_recv_destroy(instance_);
	NDIlib_destroy();
}
