#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb_image_write.h"

Image::Image(const char* filename, int channel_force) {
	if (read(filename, channel_force)) {
		printf("Successfully loaded %s\n", filename);
	} else {
		printf("Failed to load %s\n", filename);
	}
}

Image::Image(int w, int h, int channels) : _w(w), _h(h), _channels(channels){
	_size = w * h * channels;
	_data = new unsigned char[size];
}

Image::Image(const Image& img) : Image(img.w, img.h, img.channels) {
	memcpy(_data, img.data, _size);
}

Image::~Image() {
    stbi_image_free(data);
}

bool Image::read(const char* filename, int channel_force) {
	_data = stbi_load(filename, &_w, &_h, &_channels, channel_force);
	_channels = channel_force == 0 ? _channels : channel_force;
	_size = _w * _h * _channels;
	return data != NULL;
}

bool Image::write(const char *filename) {
    return stbi_write_jpg(filename, _w, _h, _channels, _data, 100);
}
