#include "ofxNDIRouter.h"

using namespace ofxNDI;

bool ofxNDIRouter::setup(const std::string &name, const std::string &group)
{
	NDIlib_routing_create_t info = {
		name.c_str(),
		group.c_str()
	};
	instance_ = NDIlib_routing_create(&info);
	return instance_ != nullptr;
}
bool ofxNDIRouter::setRoute(const Source &source) const
{
	auto src = toV1(source);
	return NDIlib_routing_change(instance_, &src);
}
Source ofxNDIRouter::getSource() const
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

