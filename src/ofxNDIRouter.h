#pragma once

#include "ofxNDI.h"
#include <string>
#include <Processing.NDI.Lib.h>

namespace ofxNDI {
class Router
{
public:
	using Instance = NDIlib_routing_instance_t;
	~Router();

	bool setup(const std::string &name, const std::string &group="");
	bool setRoute(const ofxNDI::Source &source) const;

	void addConnectionMetadata(const std::string &metadata, int64_t timecode=NDIlib_send_timecode_synthesize) const;
	void clearConnectionMetadata() const;

	ofxNDI::Source getSource() const;
	int getNumConnections(uint32_t timeout=0) const;
	bool clear() const;
private:
	Instance instance_=nullptr;
};
}

using ofxNDIRouter = ofxNDI::Router;
