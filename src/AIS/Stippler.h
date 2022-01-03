//
// Created by Stuart Rolph on 12/26/21.
//

#ifndef SCRIBBLE_STIPPLER_H
#define SCRIBBLE_STIPPLER_H


#include "Image.h"
#include "Canvas.h"
#include <queue>

class Stippler {

    struct Arc {
        // Counter-clockwise
        float from, to;

        float size() const;
        bool contains(float a) const;
        bool overlaps_with(const Arc& a) const;
        void combine_with(const Arc& a);
    };

    struct Disk {
        float x, y;
        float radius;
        std::vector<Arc> subtracted_arcs;

        float getRandomAngleFromAvailableRange();
        void subtractFromAvailableRange(const Disk& D, float alpha);
        bool hasAvailableRange();
    };

    Image _base_img;
    Canvas _disks_buffer;

    bool changeDiskRadiusBasedOnLocalDensity(Disk& D, int K);
    bool changeDiskRadiusBasedOnLocalDensity(Disk& D, const Disk& Q, int K, float alpha);
    void drawDisk(const Disk& D);
    bool OverLap(const Disk& D);
    float localDensity(const Disk& D);
    int totalDensity() const;

public:
    Stippler(const Image& img) : _base_img(img), _disks_buffer(img.w, img.h) { _base_img.grayscale(0.299, 0.587, 0.114); }

    std::vector<float> Stipple(int K);

};


#endif //SCRIBBLE_STIPPLER_H
