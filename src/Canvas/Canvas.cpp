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
    for (int i = 0; i < _img.size; i += _img.channels)
        memcpy(img.data + i, _bg_color, _img.channels);
}

void Canvas::point(float x, float y, float r) {
    if (r <= 0.5f && 0 <= x && x < _img.w && 0 <= y && y < _img.h) {
        memcpy(_img.pixel(std::floor(x), std::floor(y)), _draw_color, _img.channels);
        return;
    }
    float x_min = std::max(0.0f, x - r);
    float x_max = std::min(_img.w - 1.0f, x + r);
    float y_min = std::max(0.0f, y - r);
    float y_max = std::min(_img.h - 1.0f, y + r);
    float bound = r * r;
    if ((0 <= x_max && x_min <= _img.w - 1.0f) && (0 <= y_max && y_min <= _img.h - 1.0f)) {
        for (float y_ind = y_min; y_ind <= y_max; y_ind += 1.0f) {
            float y_off = y_ind - y;
            for (float x_ind = x_min; x_ind <= x_max; x_ind += 1.0f) {
                float x_off = x_ind - x;
                if (x_off * x_off + y_off * y_off <= bound)
                    memcpy(_img.pixel((int)std::floor(x_ind), (int)std::floor(y_ind)), _draw_color, _img.channels);
            }
        }
    }
}

bool Canvas::point_overlap(float x, float y, float r) {
    if (r <= 0.5f && 0 <= x && x < _img.w && 0 <= y && y < _img.h) {
        return memcmp(_img.pixel(std::floor(x), std::floor(y)), _bg_color, _img.channels);
    }
    float x_min = std::max(0.0f, x - r);
    float x_max = std::min(_img.w - 1.0f, x + r);
    float y_min = std::max(0.0f, y - r);
    float y_max = std::min(_img.h - 1.0f, y + r);
    float bound = r * r;
    if ((0 <= x_max && x_min <= _img.w - 1.0f) && (0 <= y_max && y_min <= _img.h - 1.0f)) {
        for (float y_ind = y_min; y_ind <= y_max; y_ind += 1.0f) {
            float y_off = y_ind - y;
            for (float x_ind = x_min; x_ind <= x_max; x_ind += 1.0f) {
                float x_off = x_ind - x;
                if (x_off * x_off + y_off * y_off <= bound)
                    if (memcmp(_img.pixel(std::floor(x), std::floor(y)), _bg_color, _img.channels) != 0)
                        return true;
            }
        }
    }
    return false;
}




