//
// Created by Stuart Rolph on 12/23/21.
//

#include "SVGWriter.h"
#include <cmath>


void SVGWriter::point(float x, float y, float radius, const std::string &stroke) {
    elements << "\t<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << radius << "\" fill=\"" << stroke << "\"/>\n";
}

void SVGWriter::points(const std::vector<float> &points, float radius, const std::string& stroke) {
    if (points.size() % 2 == 1) {
        printf("SVGWriter::points: Vector must contain even number of values (contains %d)\n", points.size());
        return;
    }
    for (int i = 0; i < points.size(); i += 2) {
        point(points.at(i), points.at(i + 1), radius, stroke);
    }
}

void SVGWriter::path(const std::vector<float> &points, int width, const std::string& stroke) {
    if (points.size() % 2 == 1) {
        printf("SVGWriter::path: Vector must contain even number of values (contains %d)\n", points.size());
        return;
    } else if (points.size() < 4) {
        printf("SVGWriter::path: Path must contain at least 2 points (contains %d)\n", points.size() / 2);
        return;
    }
    elements
        << "\t<path d=\""
        << "M " << points.at(0) << " " << points.at(1) << " ";
    for (int i = 2; i < points.size(); i += 2) {
        elements << "L " << points.at(i) << " " << points.at(i + 1) << " ";
    }
    elements
        << "\" "
        << "stroke=\"" << stroke << "\" "
        << "stroke-width=\"" << width << "\" "
        << "fill=\"none\" />\n";
}

void SVGWriter::curve_path(const std::vector<float> &points, int width, const std::string &stroke) {
    if (points.size() % 2 == 1) {
        printf("SVGWriter::path: Vector must contain even number of values (contains %d)\n", points.size());
        return;
    } else if (points.size() < 8) {
        printf("SVGWriter::path: Path must contain at least 4 points (contains %d)\n", points.size() / 2);
        return;
    }

    const float alpha = 0.5f;
    float t[4]; t[0] = 0.0f;

    float c1, c2, d1, d2, M1_x, M1_y, M2_x, M2_y, Q1_x, Q1_y, Q2_x, Q2_y;

    elements
        << "\t<path d=\"";

    bool firstPoint = true;
    for (int p = 0; p < points.size() - 6; p += 2) {
        for (int i = 0; i < 3; i++) {
            float xi = points.at(p + i * 2);
            float yi = points.at(p + i * 2 + 1);
            float xi1 = points.at(p + i * 2 + 2);
            float yi1 = points.at(p + i * 2 + 3);
            t[i + 1] = std::powf(std::sqrt(std::powf(xi1 - xi, 2) + std::powf(yi1 - yi, 2)), alpha) + t[i];
        }

        if (firstPoint) {
            firstPoint = false;
            c1 = (t[2] - t[1]) / (t[2] - t[0]);
            c2 = (t[1] - t[0]) / (t[2] - t[0]);

            M1_x = (t[2] - t[1]) * (c1 * (points.at(2) - points.at(0)) / (t[1] - t[0]) + c2 * (points.at(4) - points.at(2)) / (t[2] - t[1]));
            M1_y = (t[2] - t[1]) * (c1 * (points.at(3) - points.at(1)) / (t[1] - t[0]) + c2 * (points.at(5) - points.at(3)) / (t[2] - t[1]));

            Q1_x = points.at(2) + M1_x / 3;
            Q1_y = points.at(3) + M1_y / 3;

            elements
                << "M " << points.at(2) << "," << points.at(3) << " "
                << "C " << Q1_x << "," << Q1_y << " ";
        } else {
            elements << "S ";
        }

        d1 = (t[3] - t[2]) / (t[3] - t[1]);
        d2 = (t[2] - t[1]) / (t[3] - t[1]);

        M2_x = (t[2] - t[1]) * (d1 * (points.at(p + 4) - points.at(p + 2)) / (t[2] - t[1]) + d2 * (points.at(p + 6) - points.at(p + 4)) / (t[3] - t[2]));
        M2_y = (t[2] - t[1]) * (d1 * (points.at(p + 5) - points.at(p + 3)) / (t[2] - t[1]) + d2 * (points.at(p + 7) - points.at(p + 5)) / (t[3] - t[2]));

        Q2_x = points.at(p + 4) - M2_x / 3;
        Q2_y = points.at(p + 5) - M2_y / 3;

        elements
            << Q2_x << "," << Q2_y
            << " " << points.at(p + 4) << "," << points.at(p + 5) << " ";
    }

    elements
            << "\" "
            << "stroke=\"" << stroke << "\" "
            << "stroke-width=\"" << width << "\" "
            << "fill=\"none\" />\n";

}

void SVGWriter::write(const char *filename) {
    // Output file stream
    std::ofstream of;
    of.open(filename);

    // SVG prefix
    of << "<svg xmlns=\"http://www.w3.org/2000/svg\"";
    if (_w) of << "width=\"" << _w << "\" height=\"" << _h << "\"";
    of << ">\n";

    // Elements
    of << elements.str();

    // SVG postfix
    of << "</svg>";

    // Close file
    of.close();
}

