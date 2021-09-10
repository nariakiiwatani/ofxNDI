#pragma once

#include "ofxNDI.h"
#include <Processing.NDI.Advanced.h>

namespace ofxNDI {
namespace Frame {
template<typename SrcType> uint64_t calcDataSize(const SrcType &src);
template<typename SrcType, typename DstType> DstType encode(const SrcType &src, void *data_ptr, uint64_t data_size);
template<typename SrcType, typename DstType> bool decode(const SrcType &src, DstType &dst);

}

using VideoFrame = NDIlib_video_frame_v2_t;
using AudioFrame = NDIlib_audio_frame_v3_t;
using AudioFrameInterleaved = NDIlib_audio_frame_interleaved_32f_t;
using MetadataFrame = NDIlib_metadata_frame_t;
}
