//
// Created by Stuart Rolph on 12/18/21.
//

#include <iostream>
#include "Image/Image.h"
#include "SVGWriter/SVGWriter.h"

int main() {
//    Image ven(100, 100);
//    printf("%d %d %d %d\n", ven.w, ven.h, ven.channels, ven.size);
//    uint8_t* p = ven.pixel(5, 10);
//    p[0]=0; p[1]=255; p[2]=255;
//    ven.write("../modified.jpg");

    SVGWriter svg;
    std::vector<float> v;

    int points = 20;
    int radius = 100;

    float angle = 2 * 3.14159 / points;

    for (int i = 0; i < points; i++) {
        float r = radius * (1.5 - (float)rand() / RAND_MAX);
        v.push_back(radius * 1.5 + r * cos(i * angle)); v.push_back(radius * 1.5 + r * sin(i * angle));
    }

    svg.points(v, 2);
    svg.path(v);
    svg.curve_path(v, 2, "red");
    svg.write("../test.svg");

    return 0;
}