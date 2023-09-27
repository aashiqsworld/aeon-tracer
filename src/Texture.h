#ifndef TEXTURE_H
#define TEXTURE_H

#include "Common.h"

using namespace std;

class Texture {
public:
    virtual ~Texture() = default;

    virtual color value(double u, double v, const point3& p) const = 0;
};

class SolidColor : public Texture {
public:
    SolidColor(color c) : color_value(c) {}

    SolidColor(double red, double green, double blue) : SolidColor(color(red, green, blue)) {}

    color value(double u, double v, const point3& p) const override {
        return color_value;
    }

private:
    color color_value;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture(double _scale, shared_ptr<Texture> _even, shared_ptr<Texture> _odd)
    : inv_scale(1.0 / _scale), even(_even), odd(_odd) {}

    CheckerTexture(double _scale, color c1, color c2)
    : inv_scale(1.0 / _scale),
    even(make_shared<SolidColor>(c1)),
    odd(make_shared<SolidColor>(c2))
    {}

    color value(double u, double v, const point3& p) const override {
        auto xInteger = static_cast<int>(std::floor(inv_scale * p.x()));
        auto yInteger = static_cast<int>(std::floor(inv_scale * p.y()));
        auto zInteger = static_cast<int>(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double inv_scale;
    shared_ptr<Texture> even;
    shared_ptr<Texture> odd;
};

#endif
