//
// Created by Stuart Rolph on 12/19/21.
//

#include <algorithm>
#include <cmath>
#include "Canvas.h"

Canvas::Canvas(const Image &img) : _img(img) {
    init(img.channels);
}

Canvas::Canvas(int w, int h, int channels) : _img(w, h, channels) {
    init(channels);
}

void Canvas::init(int channels) {
    _draw_color = new uint8_t[channels];
    _bg_color = new uint8_t[channels];
    stroke(255);
    background(0);
    thickness(1);
}

Canvas::~Canvas() {
    delete[] _draw_color;
    delete[] _bg_color;
}

void Canvas::stroke(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (_img.channels < 3) {
        printf("Attempted to set RGB(A) stroke for %d channel image.\n", _img.channels);
        return;
    }
    _draw_color[0] = r;
    _draw_color[1] = g;
    _draw_color[2] = b;
    if (_img.channels == 4) {
        _draw_color[3] = a;
    }
}

void Canvas::stroke(uint8_t b, uint8_t a) {
    _draw_color[0] = b;
    switch(_img.channels) {
        case 4:
            _draw_color[3] = a;
        case 3:
            _draw_color[2] = b;
            _draw_color[1] = b;
            break;
        case 2:
            _draw_color[1] = a;
    }
}

void Canvas::background(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (_img.channels < 3) {
        printf("Attempted to set RGB(A) background for %d channel image.\n", _img.channels);
        return;
    }
    _bg_color[0] = r;
    _bg_color[1] = g;
    _bg_color[2] = b;
    if (_img.channels == 4) {
        _bg_color[3] = a;
    }
}

void Canvas::background(uint8_t b, uint8_t a) {
    _bg_color[0] = b;
    switch(_img.channels) {
        case 4:
            _bg_color[3] = a;
        case 3:
            _bg_color[2] = b;
            _bg_color[1] = b;
            break;
        case 2:
            _bg_color[1] = a;
    }
}

void Canvas::point(int x, int y) {
    int x_min = std::max(0, x - _thickness);
    int x_max = std::min(_img.w - 1, x + _thickness);
    int y_min = std::max(0, y - _thickness);
    int y_max = std::min(_img.h - 1, y + _thickness);
    if ((0 <= x_max && x_min < _img.w) && (0 <= y_max && y_min < _img.h)) {
        float r_sq = std::powf(_thickness + 0.5f, 2);
        for (int y_ind = y_min; y_ind <= y_max; y_ind++) {
            int y_off = y_ind - y;
            for (int x_ind = x_min; x_ind <= x_max; x_ind++) {
                int x_off = x_ind - x;
                if (x_off * x_off + y_off * y_off < r_sq)
                    memcpy(_img.pixel(x_ind, y_ind), _draw_color, _img.channels);
            }
        }
    }
}




