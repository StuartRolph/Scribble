
class Image {
	unsigned char* _data;
	unsigned int _size;
	int _w, _h, _channels;

	public:

	unsigned char* const& data = _data;
	const unsigned int& size = _size;
	const int &w = _w, &h = _h, &channels = _channels;


	Image(const char* filename, int channel_force = 0);
	Image(int w, int h, int channels = 3);
	Image(const Image& img);
	~Image();

	private:
	bool read(const char* filename, int channel_force);
	bool write(const char* filename);

};