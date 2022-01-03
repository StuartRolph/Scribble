#include "Image.h"
#include <cmath>
#include <algorithm>

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
	_data = new uint8_t[size];
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
    return stbi_write_png(filename, _w, _h, _channels, _data, _w * _channels);
}

Image& Image::grayscale(float rx, float gx, float bx) {
    if (channels < 3) {
        printf("Image::grayscale: Image %p has fewer than three channels. Cannot be converted to grayscale.", this);
        return *this;
    }

    int new_channels = channels - 2;
    int new_size = w * h * new_channels;
    uint8_t* new_data = new uint8_t[new_size];

    int j = 0;
    for (int i = 0; i < new_size; i += new_channels) {
        new_data[i] = std::min(255, (int)std::round(_data[j] * rx + _data[j + 1] * gx + _data[j + 2] * bx));
        j += channels;
    }

    delete[] _data;
    _data = new_data;
    _size = new_size;
    _channels = new_channels;

    return *this;
}
