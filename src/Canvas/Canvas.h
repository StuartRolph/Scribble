//
// Created by Stuart Rolph on 12/19/21.
//

#ifndef SCRIBBLE_CANVAS_H
#define SCRIBBLE_CANVAS_H


#include "Image.h"

class Canvas {
    Image _img;
    uint8_t *_draw_color, *_bg_color;
    int _thickness;

public:
    const Image& img = _img;

    // Canvas constructor / destructor
    Canvas(const Image& img);
    Canvas(int w, int h, int channels = 3);
    ~Canvas();

    // Pen settings
    void stroke(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    void stroke(uint8_t b, uint8_t a = 255);
    void thickness(int t) { _thickness = t - 1; }

    // Background settings
    void background(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    void background(uint8_t b, uint8_t a = 255);

    // Draw functions
    void point(int px, int py);
    void point(int px, int py, int r);
    bool point_overlap(int px, int py, int r);
    //void line(int x0, int y0, int x1, int y1);

    bool save(const char* filename) { return _img.write(filename); }

private:
    void init(int channels);

};


#endif //SCRIBBLE_CANVAS_H
