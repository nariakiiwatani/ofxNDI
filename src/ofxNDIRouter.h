#pragma once

#include <string>
#include <Processing.NDI.Lib.h>

namespace ofxNDI {
class Router
{
public:
	using Instance = NDIlib_routing_instance_t;
	~Router();

	bool setup(const std::string &name, const std::string &group="");
	bool setRoute(const NDIlib_source_t &p_source);
	bool clear();
private:
	Instance instance_=nullptr;
};
}

using ofxNDIRouter = ofxNDI::Router;
