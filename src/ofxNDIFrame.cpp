#include "ofxNDIFrame.h"
#include "ofImage.h"
#include "ofSoundBuffer.h"
#include "Processing.NDI.utilities.h"

using namespace ofxNDI;

#pragma mark video
namespace {
NDIlib_FourCC_video_type_e getFourCCTypeFromOfPixelFormat(ofPixelFormat format) {
	switch(format) {
//		case OF_PIXELS_GRAY:
//		case OF_PIXELS_GRAY_ALPHA:
//		case OF_PIXELS_RGB:
//		case OF_PIXELS_BGR:
		case OF_PIXELS_RGBA: return NDIlib_FourCC_video_type_RGBA;
		case OF_PIXELS_BGRA: return NDIlib_FourCC_video_type_BGRA;
//		case OF_PIXELS_RGB565:
		case OF_PIXELS_NV12: return NDIlib_FourCC_video_type_NV12;
//		case OF_PIXELS_NV21:
		case OF_PIXELS_YV12: return NDIlib_FourCC_video_type_YV12;
		case OF_PIXELS_I420: return NDIlib_FourCC_video_type_I420;
//		case OF_PIXELS_YUY2:
		case OF_PIXELS_UYVY: return NDIlib_FourCC_video_type_UYVY;
//		case OF_PIXELS_Y:
//		case OF_PIXELS_U:
//		case OF_PIXELS_V:
//		case OF_PIXELS_UV:
//		case OF_PIXELS_VU:
		default:
			ofLogWarning("ofxNDI : this pixel format is not supported");
			return NDIlib_FourCC_video_type_RGBA;
	}
}
int getBitsPerPixel(NDIlib_FourCC_video_type_e type) {
	switch(type) {
		case NDIlib_FourCC_video_type_RGBX:
		case NDIlib_FourCC_video_type_RGBA:
		case NDIlib_FourCC_video_type_BGRX:
		case NDIlib_FourCC_video_type_BGRA: return 32;
		case NDIlib_FourCC_video_type_P216:
		case NDIlib_FourCC_video_type_PA16:
		case NDIlib_FourCC_video_type_UYVY:
		case NDIlib_FourCC_video_type_UYVA: return 16;
		case NDIlib_FourCC_video_type_YV12:
		case NDIlib_FourCC_video_type_NV12:
		case NDIlib_FourCC_video_type_I420: return 12;
		default:
			ofLogWarning("ofxNDI::VideoFrame") << "unhandled FourCC type: " << type;
			return 0;
	}
}
int getLineStrideInBytes(NDIlib_FourCC_video_type_e type, int width) {
	int bits_per_pixel = getBitsPerPixel(type);
	static const int byte_size = 8;
	return ceil(bits_per_pixel*width/(float)byte_size);
}
uint64_t getDataSizeInBytes(int width, int height, NDIlib_FourCC_video_type_e type)
{
	uint64_t ret = getLineStrideInBytes(type, width) * height;
	if(type == NDIlib_FourCC_video_type_UYVA) {
		ret += sizeof(char)*width*height;
	}
	return ret;
}
ofPixelFormat getOfPixelFormatFromFourCCType(NDIlib_FourCC_video_type_e type)
{
	ofPixelFormat format;
	switch(type) {
		case NDIlib_FourCC_video_type_RGBX:
		case NDIlib_FourCC_video_type_RGBA:	format = OF_PIXELS_RGBA;	break;
		case NDIlib_FourCC_video_type_BGRX:
		case NDIlib_FourCC_video_type_BGRA:	format = OF_PIXELS_BGRA;	break;
		case NDIlib_FourCC_video_type_UYVY:	format = OF_PIXELS_UYVY;	break;
		default:
			ofLogWarning("ofxNDI : this pixel format is not supported");
			format = OF_PIXELS_RGBA;
			break;
	}
	return format;
}

}

template<>
uint64_t Frame::calcDataSize(const ofPixels &src)
{
	return getDataSizeInBytes(src.getWidth(), src.getHeight(), getFourCCTypeFromOfPixelFormat(src.getPixelFormat()));
}

template<>
VideoFrame Frame::encode(const ofPixels &src, void *data_ptr, uint64_t data_size)
{
	if(src.getData() != data_ptr) {
		auto required_size = calcDataSize(src);
		if(data_size < required_size) {
			ofLogWarning("ofxNDI") << "data size exceeded";
		}
		else {
			memcpy(data_ptr, src.getData(), required_size);
		}
	}
	VideoFrame ret;
	ret.xres = src.getWidth();
	ret.yres = src.getHeight();
	ret.FourCC = getFourCCTypeFromOfPixelFormat(src.getPixelFormat());
	ret.p_data = (uint8_t*)data_ptr;
	ret.picture_aspect_ratio = ret.xres/(float)ret.yres;
	ret.line_stride_in_bytes = getLineStrideInBytes(ret.FourCC, ret.xres);
	return ret;
}

template<>
bool Frame::decode(const VideoFrame &src, ofPixels &dst)
{
	dst.setFromPixels(src.p_data, src.xres, src.yres, getOfPixelFormatFromFourCCType(src.FourCC));
	return true;
}

#pragma mark audio

template<>
uint64_t Frame::calcDataSize(const ofSoundBuffer &src)
{
	return sizeof(float)*src.size();
}

template<>
AudioFrameInterleaved Frame::encode(const ofSoundBuffer &src, void *data_ptr, uint64_t data_size)
{
	if(src.getBuffer().data() != data_ptr) {
		auto required_size = calcDataSize(src);
		if(data_size < required_size) {
			ofLogWarning("ofxNDI") << "data size exceeded";
		}
		else {
			memcpy(data_ptr, src.getBuffer().data(), required_size);
		}
	}
	return NDIlib_audio_frame_interleaved_32f_t(src.getSampleRate(), src.getNumChannels(), src.getNumFrames(), 0,  (float*)data_ptr);
}

template<>
bool Frame::decode(const AudioFrameInterleaved &src, ofSoundBuffer &dst)
{
	dst.allocate(src.no_samples, src.no_channels);
	dst.copyFrom(src.p_data, src.no_samples, src.no_channels, src.sample_rate);
	return true;
}

template<>
AudioFrame Frame::encode(const ofSoundBuffer &src, void *data_ptr, uint64_t data_size)
{
	auto interleaved = Frame::encode<ofSoundBuffer, AudioFrameInterleaved>(src, (void*)src.getBuffer().data(), sizeof(float)*src.size());
	AudioFrame ret;
	ret.sample_rate = src.getSampleRate();
	ret.no_channels = src.getNumChannels();
	ret.no_samples = src.getNumFrames();
	ret.channel_stride_in_bytes = src.getNumFrames()*sizeof(float);
	ret.p_data = (uint8_t*)data_ptr;

	NDIlib_audio_frame_v2_t v2_frame(ret.sample_rate, ret.no_channels, ret.no_samples, ret.timecode, reinterpret_cast<float*>(ret.p_data), src.getNumFrames()*sizeof(float));
	NDIlib_util_audio_from_interleaved_32f_v2(&interleaved, &v2_frame);
	return ret;
}

template<>
bool Frame::decode(const AudioFrame &src, ofSoundBuffer &dst)
{
	if(dst.getSampleRate() != src.sample_rate) {
		dst.setSampleRate(src.sample_rate);
	}
	if(dst.getNumChannels() != src.no_channels ||
	   dst.getNumFrames() != src.no_samples) {
		dst.allocate(src.no_samples, src.no_channels);
	}
	
	NDIlib_audio_frame_interleaved_32f_t interleaved_frame(src.sample_rate, src.no_channels, src.no_samples, src.timecode
										 , static_cast<float*>(dst.getBuffer().data()));
	
	NDIlib_audio_frame_v2_t v2_frame(src.sample_rate, src.no_channels, src.no_samples, src.timecode, reinterpret_cast<float*>(src.p_data), src.channel_stride_in_bytes, src.p_metadata, src.timestamp);
	NDIlib_util_audio_to_interleaved_32f_v2(&v2_frame, &interleaved_frame);
	return true;
}

#pragma mark metadata

template<>
uint64_t Frame::calcDataSize(const std::string &src)
{
	return src.size()+1;
}

template<>
MetadataFrame Frame::encode(const std::string &src, void *data_ptr, uint64_t data_size)
{
	if(src.data() != data_ptr) {
		memcpy(data_ptr, src.data(), data_size-1);
		((uint8_t*)data_ptr)[data_size-1] = '\0';
	}
	MetadataFrame ret;
	ret.length = data_size;
	ret.p_data = (char*)data_ptr;
	return ret;
}

template<>
bool Frame::decode(const MetadataFrame &src, std::string &dst)
{
	dst = src.p_data;
	return true;
}
