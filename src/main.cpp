//
// Created by Stuart Rolph on 12/18/21.
//

#include <iostream>
#include "Image/Image.h"
#include "SVGWriter/SVGWriter.h"
#include "Canvas/Canvas.h"
#include "AIS/Stippler.h"

#define randf ((float)rand() / RAND_MAX)
#define TAU (2.0f * M_PI)

int main() {


    srand(time(nullptr));
    rand();

    Canvas can(300, 300);
    float x1 = 150;
    float y1 = 150;
    float r1 = randf * 50.0f;
    float r2 = randf * 50.0f;
    float alpha = randf * TAU;

    float x2 = x1 + (r1 + r2) * cos(alpha);
    float y2 = y1 + (r1 + r2) * sin(alpha);

    can.point(x1, y1, r1);
    can.stroke(255, 0, 0);
    std::cout << "Overlap: " << can.point_overlap(x2, y2, r2) << "\n";
    can.point(x2, y2, r2);


    can.save("../float_point.png");


    return 0;
}