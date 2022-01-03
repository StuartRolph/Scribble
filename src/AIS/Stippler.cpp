//
// Created by Stuart Rolph on 12/26/21.
//

#include "Stippler.h"
#include <cmath>

constexpr float TAU = 2.0 * M_PI;

float Stippler::Arc::size() const {
    if (from > to) {
        return to + (TAU - from);
    }
    return to - from;
}

bool Stippler::Arc::contains(float a) const {
    // Special case: Arc crosses 0 angle
    if (from > to) {
        return from <= a || a <= to;
    }
    return from <= a && a <= to;
}

bool Stippler::Arc::overlaps_with(const Stippler::Arc& a) const {
    return contains(a.from) || contains(a.to) || a.contains(from);
}

void Stippler::Arc::combine_with(const Stippler::Arc& a) {
    bool OtherStartInsideThis = contains(a.from);
    bool OtherEndInsideThis = contains(a.to);
    bool ThisStartInsideOther = a.contains(from);
    bool ThisEndInsideOther = a.contains(to);

    if (OtherStartInsideThis && OtherEndInsideThis && ThisStartInsideOther && ThisEndInsideOther) {
        from = 0;
        to = TAU;
    } else if (ThisStartInsideOther && ThisEndInsideOther) {
        from = a.from;
        to = a.to;
    } else if (OtherStartInsideThis && ThisEndInsideOther) {
        to = a.to;
    } else if (OtherEndInsideThis && ThisStartInsideOther) {
        from = a.from;
    }

}

float Stippler::Disk::getRandomAngleFromAvailableRange() {
    std::sort(subtracted_arcs.begin(), subtracted_arcs.end(), [] (Arc a, Arc b) { return a.from < b.from; });
    float available_range_size = TAU;
    for (Arc a : subtracted_arcs) {
        available_range_size -= a.size();
    }
    float rand_angle = (float)rand() / RAND_MAX * available_range_size;
    if (subtracted_arcs.size() == 0) return rand_angle;
    for (Arc a : subtracted_arcs) {
        if (rand_angle < a.from) {
            return rand_angle;
        } else if (a.contains(rand_angle)) {
            rand_angle += (a.size());
        }
    }
    return rand_angle;
    //printf("Stippler::Disk::getRandomAngleFromAvailableRange: Available angle not found. This should not occur.\n");
    //return 0;
}

inline float mod(float x, float y) {
    return fmod(x, y) + (x < 0 ? y : 0);
}

void Stippler::Disk::subtractFromAvailableRange(const Stippler::Disk& D, float alpha) {
    float dist = radius + D.radius;
    float cosBeta = std::max(-1.0f, std::min(1.0f, dist / (2.0f * D.radius)));
    float beta = std::acosf(cosBeta);
    //float beta = std::asinf(D.radius / dist);
    Arc t = {mod(alpha - beta, TAU), mod(alpha + beta, TAU)};

    for (int i = subtracted_arcs.size() - 1; i >= 0; i--) {
        if (t.overlaps_with(subtracted_arcs.at(i))) {
            t.combine_with(subtracted_arcs.at(i));
            subtracted_arcs.erase(subtracted_arcs.begin() + i);
        }
    }
    subtracted_arcs.push_back(t);
}

bool Stippler::Disk::hasAvailableRange() {
    if (subtracted_arcs.empty()) return true;
    return !(subtracted_arcs.at(0).from == 0 && subtracted_arcs.at(0).to == TAU);
}

void Stippler::drawDisk(const Stippler::Disk& D) {
    float r = std::max(1.0f, std::floor(D.radius));
    _disks_buffer.point(std::round(D.x), std::round(D.y), r);
//    if (r > 20) {
//        printf("drawDisk at %f, %f, r:%f\n", D.x, D.y, r);
//    }
}

bool Stippler::OverLap(const Stippler::Disk& D) {
    float r = std::max(1.0f, std::floor(D.radius));
    return _disks_buffer.point_overlap(D.x, D.y, r);
}

bool Stippler::changeDiskRadiusBasedOnLocalDensity(Stippler::Disk& D, int K) {
    float diskDensity = localDensity(D);
    if (diskDensity == 0) return false;
    float ratioR = std::sqrtf((float)K / diskDensity);
    constexpr float epsilon = 0.01;
    int i = 0;
    while (std::abs(ratioR - 1.0f) > epsilon && i < 6) {
        float rn = ratioR * D.radius;
        D.radius = rn;
        diskDensity = localDensity(D);
        ratioR = std::sqrtf((float)K / diskDensity);
        i++;
    }
    return true;
}

bool Stippler::changeDiskRadiusBasedOnLocalDensity(Stippler::Disk& D, const Stippler::Disk& Q, int K, float alpha) {
    float diskDensity = localDensity(D);
    if (diskDensity == 0) return false;
    float ratioR = std::sqrtf((float)K / diskDensity);
    constexpr float epsilon = 0.01;
    int i = 0;
    while (std::abs(ratioR - 1.0f) > epsilon && i < 6) {
        float rn = ratioR * D.radius;
        D.x = (Q.x + (Q.radius + rn) * cos(alpha));
        D.y = (Q.y + (Q.radius + rn) * sin(alpha));
        D.radius = rn;
        diskDensity = localDensity(D);
        if (diskDensity == 0) return false;
        ratioR = std::sqrtf((float)K / diskDensity);
        //if (ratioR > 3.0f) ratioR /= 2.0f;
        i++;
    }
    return true;
}

float Stippler::localDensity(const Stippler::Disk& D){
    //int px = std::round(D.x);
    //int py = std::round(D.y);
    //int r = D.radius;

    int sum = 0;

//    if (r == 0 && 0 <= px && px < _base_img.w && 0 <= py && py < _base_img.h) {
//        return 255 - _base_img.pixel(px, py)[0];
//    }
    int x_min = std::max(0.0f, D.x - D.radius);
    int x_max = std::min(_base_img.w - 1.0f, D.x + D.radius);
    int y_min = std::max(0.0f, D.y - D.radius);
    int y_max = std::min(_base_img.h - 1.0f, D.y + D.radius);
    //int bound = r * r + r;
    if ((0 <= x_max && x_min < _base_img.w) && (0 <= y_max && y_min < _base_img.h)) {
        for (int y_ind = y_min; y_ind <= y_max; y_ind++) {
            //int y_off = y_ind - py;
            for (int x_ind = x_min; x_ind <= x_max; x_ind++) {
                //int x_off = x_ind - px;
                //if (x_off * x_off + y_off * y_off < bound)
                sum += (255 - _base_img.pixel(x_ind, y_ind)[0]);
            }
        }
    }
    int squareArea = (x_max - x_min) * (y_max - y_min);
    float diskArea = D.radius * D.radius * M_PI;
    return sum * diskArea / squareArea;
}

int Stippler::totalDensity() const {
    int sum = 0;
    for (int i = 0; i < _base_img.size; i += _base_img.channels) {
        sum += (255 - _base_img.data[i]);
    }
    return sum;
}

std::vector<float> Stippler::Stipple(int K) {

    std::queue<Disk> _disk_queue;
    std::vector<float> _output_point_list;
    float TotalDensity = totalDensity();
    float r1 = (std::sqrt(_base_img.w * _base_img.h / (TotalDensity / K)));
    Disk D = {(float)(rand() % _base_img.w), (float)(rand() % _base_img.h), r1};
    changeDiskRadiusBasedOnLocalDensity(D, K);
    _disk_queue.push(D);
    _output_point_list.push_back(D.x);
    _output_point_list.push_back(D.y);
    drawDisk(D);
    while(!_disk_queue.empty()) {
        Disk Q = _disk_queue.front();
        _disk_queue.pop();
        int attempts = 10;
        while(Q.hasAvailableRange() && attempts --> 0) {
            float alpha = Q.getRandomAngleFromAvailableRange();
            Disk P = {(Q.x + (Q.radius + r1) * cosf(alpha)), (Q.y + (Q.radius + r1) * sin(alpha)), r1};
            bool insideImage = 0 <= P.x && P.x < _disks_buffer.img.w && 0 <= P.y && P.y < _disks_buffer.img.h;
            bool hasDensity = changeDiskRadiusBasedOnLocalDensity(P, Q, K, alpha);
            bool overlaps = OverLap(P);
            if (insideImage && !overlaps) {
                _disk_queue.push(P);
                if (hasDensity) {
                    _output_point_list.push_back(P.x);
                    _output_point_list.push_back(P.y);
                }
                drawDisk(P);
            }
            Q.subtractFromAvailableRange(P, alpha);
        }
    }

    _disks_buffer.save("../stipple.jpg");
    return _output_point_list;
}
