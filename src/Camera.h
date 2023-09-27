#ifndef CAMERA_H
#define CAMERA_H

#include <fstream>
#include "Common.h"
#include "Hittable.h"
#include "Material.h"
#include "HittableList.h"

class Camera {
public:
    point3 origin;
    int image_width, image_height;
    int max_depth = 50;

    Camera(
        point3 lookfrom,
        point3 lookat,
        vec3 vup,
        double vfov,
        int _width,
        double aspect_ratio,
        double aperture,
        double focus_dist,
        double _time0 = 0,
        double _time1 = 0
    ) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        image_width = _width;
        image_height = static_cast<int>(image_width / aspect_ratio);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

        lens_radius = aperture / 2;
        time0 = _time0;
        time1 = _time1;
    }

    Ray get_ray(double s, double t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return Ray(
            origin + offset,
            lower_left_corner + s*horizontal + t*vertical - origin - offset,
            random_double(time0, time1));
    }

    color ray_color(const Ray& r, const Hittable& world, int depth)
    {
        hit_record rec;

        // if we've exceeded the ray bounce limit, no more light is gathered
        if(depth <= 0)
            return color(0, 0, 0);

        if(world.hit(r, Interval(0, infinity), rec))
        {
            Ray scattered;
            color attenuation;
            if(rec.mat_ptr->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, world, depth-1);
            return color(0, 0, 0);
        }
        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
    }

    void render(char* filename, int width, int height, const HittableList& world, int
    samples_per_pixel)
    {
        time_t start = time(0);
        double estimated_time = 0.0;
        std::ofstream out(filename);

        out << "P3\n" << width << ' ' << height << "\n255\n";

        for (int j = height-1; j >= 0; --j)
        {
            double minutes_remaining = estimated_time * j / 60.0;

            std::clog << "\rEstimated time remaining: " << minutes_remaining << " minutes\t\t" <<
                      std::flush;
            for (int i = 0; i < width; ++i)
            {
                color pixel_color(0, 0, 0);
                for(int s = 0; s < samples_per_pixel; ++s)
                {
                    auto u = (i + random_double()) / (width-1);
                    auto v = (j + random_double()) / (height-1);
                    Ray r = get_ray(u, v);
                    pixel_color += ray_color(r, world, max_depth);
                }
                pixel_color /= samples_per_pixel;
//			write_color(std::cout, pixel_color, samples_per_pixel);

                out << static_cast<int>(pixel_color.x() * 255.999) << ' ' <<
                    static_cast<int>(pixel_color.y() * 255.999) << ' ' <<
                    static_cast<int>(pixel_color.z() * 255.999) << '\n';
            }
            estimated_time = difftime(time(0), start);
            start = time(0);
        }
        std::clog << "\nDone.\n";
    }

private:
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
    double time0, time1;
};

#endif