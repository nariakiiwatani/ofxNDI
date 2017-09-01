#pragma once

#include <Processing.NDI.structs.h>
#include "ofPixels.h"
#include "ofSoundBuffer.h"

namespace ofxNDI {
	static void decode(NDIlib_video_frame_v2_t frame, ofPixels &dst) {
		dst.setFromPixels(frame.p_data, frame.xres, frame.yres, frame.line_stride_in_bytes/frame.xres);
	}
	static void decode(NDIlib_audio_frame_v2_t frame, ofSoundBuffer &dst) {
		dst.copyFrom(frame.p_data, frame.no_samples, frame.no_channels, frame.sample_rate);
	}
}
