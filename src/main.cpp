//
// Created by Stuart Rolph on 12/18/21.
//

#include <iostream>
#include "Image/Image.h"
#include "SVGWriter/SVGWriter.h"
#include "Canvas/Canvas.h"

int main() {

    srand(time(nullptr));

    /*SVGWriter svg;
    std::vector<float> v;

    int points = 20;
    int radius = 100;
    float amp = 0.5;

    float angle = 2 * 3.14159 / points;

    srand(5);

    for (int i = 0; i < points; i++) {
        float r = radius * (1.0f + amp / 2 - amp * (float)rand() / RAND_MAX);
        v.push_back(radius * 1.5 + r * cos(i * angle)); v.push_back(radius * 1.5 + r * sin(i * angle));
    }

    svg.points(v, 2);
    svg.path(v);
    svg.curve_path(v, 2, "red");
    svg.write("../test.svg");*/



    Canvas can(500, 500);

    for (int i = 0; i < 1000; i++) {
        int size = rand() % 50 + 5;
        int x = rand() % can.img.w;
        int y = rand() % can.img.h;

        if (!can.point_overlap(x, y, size)) {
            can.point(x, y, size);
        }

    }

    can.save("../canvas.jpg");

    return 0;
}