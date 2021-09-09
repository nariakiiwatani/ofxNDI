#include "ofxNDIRouter.h"

using namespace ofxNDI;

bool ofxNDIRouter::setup(const std::string &name, const std::string &group)
{
	NDIlib_routing_create_t info = {
		name.c_str(),
		group.c_str()
	};
	instance_ = NDIlib_routing_create_v2(&info);
	return instance_ != nullptr;
}
bool ofxNDIRouter::setRoute(const Source &source) const
{
	auto src = toV1(source);
	return NDIlib_routing_change(instance_, &src);
}

void ofxNDIRouter::addConnectionMetadata(const std::string &metadata, int64_t timecode) const
{
	const NDIlib_metadata_frame_t data = {
		static_cast<int>(metadata.length()+1),
		timecode,
		const_cast<char*>(metadata.c_str())
	};
	NDIlib_routing_add_connection_metadata(instance_, &data);
}
void ofxNDIRouter::clearConnectionMetadata() const
{
	NDIlib_routing_clear_connection_metadata(instance_);
}

Source ofxNDIRouter::getSourceName() const
{
	return toV2(*NDIlib_routing_get_source_name(instance_));
}
int ofxNDIRouter::getNumConnections(uint32_t timeout) const
{
	return NDIlib_routing_get_no_connections(instance_, timeout);
}
bool ofxNDIRouter::clear() const
{
	return NDIlib_routing_clear(instance_);
}

ofxNDIRouter::~Router()
{
	NDIlib_routing_destroy(instance_);
	NDIlib_destroy();
}

