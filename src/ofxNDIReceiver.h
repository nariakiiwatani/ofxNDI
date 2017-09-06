#pragma once

#include "ofConstants.h"
#include <Processing.NDI.Lib.h>
#include <string>
#include <vector>

namespace ofxNDI {
namespace Recv {
		
class Receiver
{
public:
	~Receiver();

	struct Source {
		Source(){}
		Source(const NDIlib_source_t &src):ndi_name(src.p_ndi_name),ip_address(src.p_ip_address){}
		operator NDIlib_source_t() const {
			return NDIlib_source_t{ndi_name.c_str(), ip_address.c_str()};
		}
		std::string ndi_name="";
		std::string ip_address="";
	};

	struct Settings {
		Settings(){}
		NDIlib_recv_color_format_e color_format=NDIlib_recv_color_format_e_RGBX_RGBA;
		NDIlib_recv_bandwidth_e bandwidth=NDIlib_recv_bandwidth_highest;
		bool deinterlace=false;
	};
	enum class Location {
		LOCAL,REMOTE,BOTH
	};
	static std::vector<Source> listSources(uint32_t waittime_ms=1000, Location location=Location::BOTH, const std::string &group="", const std::vector<std::string> extra_ips={});
	
	bool setup(std::size_t index=0, const Settings &settings=Settings());
	bool setup(const Source &source, const Settings &settings=Settings());
	bool isSetup() const { return receiver_ != nullptr; }
	bool isConnected() const;
	
	NDIlib_recv_instance_t getReceiver() const { return receiver_; }
private:
	NDIlib_recv_instance_t receiver_=nullptr;
};

}}

using ofxNDIReceiver = ofxNDI::Recv::Receiver;
