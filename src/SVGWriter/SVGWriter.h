//
// Created by Stuart Rolph on 12/23/21.
//

#ifndef SCRIBBLE_SVGWRITER_H
#define SCRIBBLE_SVGWRITER_H


#include <sstream>
#include <fstream>
#include <vector>

class SVGWriter {
    std::stringstream elements;
    int _w, _h;

public:
    SVGWriter() : _w(0), _h(0) {}
    SVGWriter(int w, int h) : _w(w), _h(h) {}

    void point(float x, float y, int radius = 1, const std::string& stroke = "black");
    void points(const std::vector<float>& points, int radius = 1, const std::string& stroke = "black");
    void path(const std::vector<float>& points, int width = 1, const std::string& stroke = "black");
    void curve_path(const std::vector<float>& points, int width = 1, const std::string& stroke = "black");

    void write(const char* filename);
};


#endif //SCRIBBLE_SVGWRITER_H
