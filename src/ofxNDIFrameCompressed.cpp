#include "ofxNDIFrameCompressed.h"
#include "ofxNDIFrame.h"
#include "ofLog.h"

using namespace ofxNDI;

namespace {
// warning : dst requires size more than sizeof(NDIlib_compressed_packet_t).
// you should decide the size with calcDataSize(const CompressedFrameInfo&)
void encodeToCompressedPacket(const CompressedFrameInfo &src, NDIlib_compressed_packet_t &dst) {
	auto p = &dst;
	p->version = sizeof(NDIlib_compressed_packet_t);
	p->pts = src.pts;
	p->dts = src.dts;
	p->flags = src.is_keyframe ? NDIlib_compressed_packet_t::flags_keyframe : NDIlib_compressed_packet_t::flags_none;
	p->data_size = src.data_size;
	p->extra_data_size = src.extra_data_size;
	p->fourCC = src.FourCC;
	auto s = (NDIlib_frame_scatter_t*)(p + 1);
	auto data_blocks = (const uint8_t**)(s + 1);
	data_blocks[0] = (const uint8_t*)p;
	data_blocks[1] = (const uint8_t*)src.data;
	data_blocks[2] = (const uint8_t*)src.extra_data;
	data_blocks[3] = 0;
	s->p_data_blocks = data_blocks;
	auto data_blocks_size = (int*)(data_blocks+4);
	data_blocks_size[0] = sizeof(NDIlib_compressed_packet_t);
	data_blocks_size[1] = src.data_size;
	data_blocks_size[2] = src.extra_data_size;
	data_blocks_size[3] = 0;
	s->p_data_blocks_size = data_blocks_size;
}
void decodeFromCompressedPacket(const NDIlib_compressed_packet_t &src, CompressedFrameInfo &dst) {
	auto p = &src;
	dst.is_keyframe = p->flags == NDIlib_compressed_packet_t::flags_keyframe;
	dst.FourCC = p->fourCC;
	dst.pts = p->pts;
	dst.dts = p->dts;
	dst.data = p->data_size ? (uint8_t*)(p + 1) : nullptr;
	dst.data_size = p->data_size;
	dst.extra_data = p->extra_data_size ? (uint8_t*)dst.data + dst.data_size : nullptr;
	dst.extra_data_size = p->extra_data_size;
}
}

template<>
uint64_t Frame::calcDataSize(const CompressedFrameInfo &src)
{
	int scatter_length = 4; // packet_t, data, extra_data, null_end
	return sizeof(NDIlib_compressed_packet_t)+sizeof(NDIlib_frame_scatter_t)+(sizeof(uint8_t*)+sizeof(int))*scatter_length;
}

template<> uint64_t Frame::calcDataSize(const CompressedVideoInfo &src) { return calcDataSize((const CompressedFrameInfo&)src); }
template<> uint64_t Frame::calcDataSize(const CompressedAudioInfo &src) { return calcDataSize((const CompressedFrameInfo&)src); }

template<>
VideoFrame Frame::encode(const CompressedVideoInfo &src, void *data_ptr, uint64_t data_size)
{
	auto required_size = calcDataSize(src);
	if(data_size < required_size) {
		ofLogWarning("ofxNDI") << "data size exceeded";
	}
	encodeToCompressedPacket(src, *(NDIlib_compressed_packet_t*)data_ptr);
	
	VideoFrame ret;
	ret.xres = src.xres;
	ret.yres = src.yres;
	ret.FourCC = (NDIlib_FourCC_video_type_e)src.video_FourCC;
	ret.p_data = (uint8_t*)data_ptr + sizeof(NDIlib_compressed_packet_t);
	ret.data_size_in_bytes = 0;
	ret.picture_aspect_ratio = src.picture_aspect_ratio != 0 ? src.picture_aspect_ratio : ret.xres/(float)ret.yres;
	return ret;
}

template<>
bool Frame::decode(const AudioFrame &src, CompressedAudioInfo &dst)
{
	decodeFromCompressedPacket(*(const NDIlib_compressed_packet_t*)src.p_data, dst);

	dst.sample_rate = src.sample_rate;
	dst.no_channels = src.no_channels;
	dst.no_samples = src.no_samples;
	dst.audio_FourCC = (NDIlib_FourCC_audio_type_ex_e)src.FourCC;
	return true;
}

template<>
AudioFrame Frame::encode(const CompressedAudioInfo &src, void *data_ptr, uint64_t data_size)
{
	auto required_size = calcDataSize(src);
	if(data_size < required_size) {
		ofLogWarning("ofxNDI") << "data size exceeded";
	}
	encodeToCompressedPacket(src, *(NDIlib_compressed_packet_t*)data_ptr);
	
	AudioFrame ret;
	ret.sample_rate = src.sample_rate;
	ret.no_samples = src.no_samples;
	ret.no_channels = src.no_channels;
	ret.FourCC = (NDIlib_FourCC_audio_type_e)src.audio_FourCC;
	ret.p_data = (uint8_t*)data_ptr + sizeof(NDIlib_compressed_packet_t);
	ret.data_size_in_bytes = 0;
	return ret;
}

template<>
bool Frame::decode(const VideoFrame &src, CompressedVideoInfo &dst)
{
	decodeFromCompressedPacket(*(const NDIlib_compressed_packet_t*)src.p_data, dst);

	dst.xres = src.xres;
	dst.yres = src.yres;
	dst.frame_format_type = src.frame_format_type;
	dst.picture_aspect_ratio = src.picture_aspect_ratio;
	dst.video_FourCC = (NDIlib_FourCC_video_type_ex_e)src.FourCC;
	return true;
}
