#include "ofxNDIFrame.h"
#include "ofImage.h"

using namespace ofxNDI;

#pragma mark video

uint64_t VideoFrame::allocate(int width, int height, NDIlib_FourCC_type_e type)
{
	uint64_t needed_data_size = getDataSizeInBytes(width, height, type);
	if(needed_data_size != getDataSizeInBytes(xres, yres, FourCC)) {
		if(is_allocated_) free();
		p_data = new unsigned char[needed_data_size];
	}
	is_allocated_ = true;
	return needed_data_size;
}
void VideoFrame::free()
{
	delete[] p_data;
	is_allocated_ = false;
}
void VideoFrame::encode(ofPixels &&src, bool copy)
{
	switch(src.getPixelFormat()) {
		case OF_PIXELS_RGB:
		case OF_PIXELS_BGR:		src.setImageType(OF_IMAGE_COLOR_ALPHA);	break;
		default: break;
	}

	int w = (int)src.getWidth(), h = (int)src.getHeight();
	NDIlib_FourCC_type_e type = getFourCCTypeFromOfPixelFormat(src.getPixelFormat());
	if(copy) {
		uint64_t size = allocate(w, h, type);
		memcpy(p_data, static_cast<unsigned char*>(src.getData()), size);
	}
	else {
		if(is_allocated_) free();
		p_data = static_cast<unsigned char*>(src.getData());
	}
	xres = w;
	yres = h;
	FourCC = type;
	picture_aspect_ratio = w/(float)h;
	line_stride_in_bytes = getLineStrideInBytes(FourCC, xres);
}
void VideoFrame::decode(ofPixels &dst) const
{
	ofPixelFormat format;
	switch(FourCC) {
		case NDIlib_FourCC_type_RGBX:
		case NDIlib_FourCC_type_RGBA:	format = OF_PIXELS_RGBA;	break;
		case NDIlib_FourCC_type_BGRX:
		case NDIlib_FourCC_type_BGRA:	format = OF_PIXELS_BGRA;	break;
		case NDIlib_FourCC_type_UYVY:	format = OF_PIXELS_UYVY;	break;
		default:
			ofLogWarning("ofxNDI : this pixel format is not supported");
			break;
	}
	dst.setFromPixels(p_data, xres, yres, format);
}
void VideoFrame::setMetadata(const std::string &metadata)
{
	metadata_buffer_ = metadata;
	p_metadata = metadata_buffer_.c_str();
}

int VideoFrame::getBitsPerPixel(NDIlib_FourCC_type_e type)
{
	switch(type) {
		case NDIlib_FourCC_type_RGBX:
		case NDIlib_FourCC_type_RGBA:
		case NDIlib_FourCC_type_BGRX:
		case NDIlib_FourCC_type_BGRA: return 32;
		case NDIlib_FourCC_type_UYVY:
		case NDIlib_FourCC_type_UYVA: return 16;
		case NDIlib_FourCC_type_YV12:
		case NDIlib_FourCC_type_NV12:
		case NDIlib_FourCC_type_I420: return 12;
	}
}
int VideoFrame::getLineStrideInBytes(NDIlib_FourCC_type_e type, int width)
{
	int bits_per_pixel = getBitsPerPixel(type);
	static const int byte_size = 8;
	return ceil(bits_per_pixel*width/(float)byte_size);
}
uint64_t VideoFrame::getDataSizeInBytes(int width, int height, NDIlib_FourCC_type_e type)
{
	uint64_t ret = getLineStrideInBytes(type, width) * height;
	if(type == NDIlib_FourCC_type_UYVA) {
		ret += sizeof(char)*width*height;
	}
	return ret;
}
NDIlib_FourCC_type_e VideoFrame::getFourCCTypeFromOfPixelFormat(ofPixelFormat format)
{
	switch(format) {
		//			case OF_PIXELS_GRAY:
		//			case OF_PIXELS_GRAY_ALPHA:
		//			case OF_PIXELS_RGB:
		//			case OF_PIXELS_BGR:
		case OF_PIXELS_RGBA: return NDIlib_FourCC_type_RGBA;
		case OF_PIXELS_BGRA: return NDIlib_FourCC_type_BGRA;
			//			case OF_PIXELS_RGB565:
		case OF_PIXELS_NV12: return NDIlib_FourCC_type_NV12;
			//			case OF_PIXELS_NV21:
		case OF_PIXELS_YV12: return NDIlib_FourCC_type_YV12;
		case OF_PIXELS_I420: return NDIlib_FourCC_type_I420;
			//			case OF_PIXELS_YUY2:
		case OF_PIXELS_UYVY: return NDIlib_FourCC_type_UYVY;
			//			case OF_PIXELS_Y:
			//			case OF_PIXELS_U:
			//			case OF_PIXELS_V:
			//			case OF_PIXELS_UV:
			//			case OF_PIXELS_VU:
		default:
			ofLogWarning("ofxNDI : this pixel format is not supported");
			break;
	}
}

#pragma mark audio

uint64_t AudioFrame::allocate(uint64_t samples)
{
	if(is_allocated_) free();
	p_data = new float[samples];
	is_allocated_ = true;
	return samples*sizeof(decltype(*p_data));
}
void AudioFrame::free()
{
	delete[] p_data;
	is_allocated_ = false;
}
void AudioFrame::encode(ofSoundBuffer &&src, bool copy)
{
	sample_rate = src.getSampleRate();
	no_channels = src.getNumChannels();
	no_samples = src.size();
	if(copy) {
		uint64_t size = allocate(src.size());
		memcpy(p_data, src.getBuffer().data(), size);
	}
	else {
		if(is_allocated_) free();
		p_data = static_cast<float*>(src.getBuffer().data());
	}
	channel_stride_in_bytes = src.getNumFrames()*sizeof(decltype(*p_data));
}
void AudioFrame::decode(ofSoundBuffer &dst) const
{
	dst.copyFrom(p_data, channel_stride_in_bytes/sizeof(decltype(*p_data)), no_channels, sample_rate);
}
void AudioFrame::setMetadata(const std::string &metadata)
{
	metadata_buffer_ = metadata;
	p_metadata = metadata_buffer_.c_str();
}


#pragma mark metadata

void MetadataFrame::encode(std::string &&src, bool copy)
{
	if(copy) {
		buffer_ = src;
		p_data = const_cast<char*>(buffer_.c_str());
	}
	else {
		p_data = const_cast<char*>(src.c_str());
	}
	length = src.length();
}
void MetadataFrame::decode(std::string &dst) const
{
	dst = p_data;
}
