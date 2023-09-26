//
// Created by Aashiq Shaikh on 8/26/23.
//

#ifndef INTERVAL_H
#define INTERVAL_H

#include "Common.h"

class Interval {
public:
    double min, max;

    Interval() : min(+infinity), max(-infinity) {} // Default interval is empty

    Interval(double _min, double _max) : min(_min), max(_max) {}

    Interval(const Interval& a, const Interval& b)
    : min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double size() const {
        return max - min;
    }

    Interval expand(double delta) const {
        auto padding = delta/2;
        return {min - padding, max + padding};
    }

    static const Interval empty, universe;
};

const static Interval empty     (+infinity, -infinity);
const static Interval universe  (-infinity, +infinity);

#endif
