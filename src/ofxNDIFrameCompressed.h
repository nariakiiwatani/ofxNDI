#pragma once

#include <stddef.h>
#include <Processing.NDI.Advanced.h>

namespace ofxNDI {

struct CompressedFrameInfo {
	const void *data;
	uint64_t data_size;
	bool is_keyframe=true;
	NDIlib_compressed_FourCC_type_e FourCC;
	int64_t pts=0, dts=0;
	const void *extra_data=nullptr;
	uint64_t extra_data_size=0;
	NDIlib_frame_format_type_e frame_format_type=NDIlib_frame_format_type_progressive;
};
struct CompressedVideoInfo : public CompressedFrameInfo {
	int xres, yres;
	NDIlib_FourCC_video_type_ex_e video_FourCC;
	float picture_aspect_ratio=0;
};
struct CompressedAudioInfo : public CompressedFrameInfo {
	int sample_rate, no_channels, no_samples;
	NDIlib_FourCC_audio_type_ex_e audio_FourCC=NDIlib_FourCC_audio_type_ex_AAC;
	
	CompressedAudioInfo() {
		FourCC = NDIlib_FourCC_type_AAC;
	}
};


}
