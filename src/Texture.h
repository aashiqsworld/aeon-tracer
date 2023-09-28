#ifndef TEXTURE_H
#define TEXTURE_H

#include "Common.h"
#include "Image.h"
#include "Perlin.h"

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

class ImageTexture : public Texture {
public:
    ImageTexture(const char* filename) : image(filename) {}

    color value(double u, double v, const point3& p) const override {
        // if we have no texture data, then return solid cyan as a debugging aid.
        if(image.height() <= 0) return color(1,0,0);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = Interval(0,1).clamp(u);
        v = 1.0 - Interval(0,1).clamp(v); // flip V to image coordinates
        
        auto i = static_cast<int>(u * image.width());
        auto j = static_cast<int>(v * image.height());
        auto pixel = image.pixel_data(i,j);

        auto color_scale = 1.0 / 255.0;
//        return color(u, v, 0);
        return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
    }

private:
    Image image;
};


class noise_texture : public Texture {
public:
    noise_texture() {}

    color value(double u, double v, const point3& p) const override {
        return color(1,1,1) * noise.noise(p);
    }

private:
    Perlin noise;
};

#endif
