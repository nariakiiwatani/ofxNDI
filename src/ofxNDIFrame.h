#pragma once

#include "ofxNDI.h"

namespace ofxNDI {
class VideoFrame : public NDIlib_video_frame_v2_t
{
public:
	uint64_t allocate(int width, int height, NDIlib_FourCC_video_type_e type);
	void free();
	void encode(ofPixels &&src, bool copy);
	void decode(ofPixels &dst) const;
	void setMetadata(const std::string &metadata);
	ofPixelFormat getOfPixelFormat() const;
private:
	std::string metadata_buffer_;
	bool is_allocated_=false;
	static int getBitsPerPixel(NDIlib_FourCC_video_type_e type);
	static int getLineStrideInBytes(NDIlib_FourCC_video_type_e type, int width);
	static uint64_t getDataSizeInBytes(int width, int height, NDIlib_FourCC_video_type_e type);
	static NDIlib_FourCC_video_type_e getFourCCTypeFromOfPixelFormat(ofPixelFormat format);
	static ofPixelFormat getOfPixelFormatFromFourCCType(NDIlib_FourCC_video_type_e type);
};

class AudioFrame : public NDIlib_audio_frame_v3_t
{
public:
	uint64_t allocate(uint64_t samples);
	void free();
	void encode(ofSoundBuffer &&src, bool copy);
	void decode(ofSoundBuffer &dst) const;
	void setMetadata(const std::string &metadata);
private:
	std::string metadata_buffer_;
	bool is_allocated_=false;
};
class MetadataFrame : public NDIlib_metadata_frame_t
{
public:
	void encode(std::string &&src, bool copy);
	void decode(std::string &dst) const;
private:
	std::string buffer_;
};
}
