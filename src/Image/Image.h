#ifndef SCRIBBLE_IMAGE_H
#define SCRIBBLE_IMAGE_H


#include <cstdint>

class Image {
	uint8_t* _data;
	unsigned int _size;
	int _w, _h, _channels;

	public:

    uint8_t* const& data = _data;
	const unsigned int& size = _size;
	const int &w = _w, &h = _h, &channels = _channels;

	Image(const char* filename, int channel_force = 0);
	Image(int w, int h, int channels = 3);
	Image(const Image& img);
	~Image();

    bool write(const char* filename);
    uint8_t* pixel(int x, int y) { return &_data[(y * _w + x) * _channels]; }
    Image& grayscale(float rx, float gx, float bx);

	private:
	bool read(const char* filename, int channel_force);

};
#endif // SCRIBBLE_IMAGE_H