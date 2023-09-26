#include "common.h"
#include <time.h>
#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "mesh.h"
#include "moving_sphere.h"

#include <iostream>
color ray_color(const ray& r, const hittable& world, int depth)
{
	hit_record rec;

	// if we've exceeded the ray bounce limit, no more light is gathered
	if(depth <= 0)
		return color(0, 0, 0);

	if(world.hit(r, interval(0,infinity), rec))
	{
		ray scattered;
		color attenuation;
		if(rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth-1);
		return color(0, 0, 0);
	}
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list random_scene_moving() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<moving_sphere>(
                            center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int round4(int x)
{
    return x % 4 == 0 ? x : x - x % 4 + 4;
}

// renders a ray in the scene
int main() {

    // test stuff START


    shared_ptr<material> mat;
     // diffuse
    auto albedo = vec3(0.3, 0.9, 0.4);
    mat = make_shared<lambertian>(albedo);
    auto m = make_shared<mesh>(mat);
//    m->loadObjModelAlt("teddy.obj");
//    m->printObjModel();
//    m->loadObjModel("teapot.obj");
//    auto m = make_shared<triangle>();
//    m->mat_ptr = mat;
//    vec3 v1 = vec3(0.0, 0.0, 0.0);
//    vec3 v2 = vec3(0.0, 1.0, 0.0);
//    vec3 v3 = vec3(1.0, 0.0, 1.0);
//    m->setVertices(v1, v2, v3);


//    m->loadObjModel("cube.obj");
//    m.printObjModel();

//    exit(0);


    // test stuff END

	// Image
    const char *filename = "image.bmp";
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 300;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 1;
	const int max_depth = 50;

	// World
	auto world = random_scene_moving();
//	hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));
    world.add(m);

    // Camera
	point3 lookfrom(10,5,3);
	point3 lookat(0,0,0);
	vec3 vup(0,1,0);
	auto dist_to_focus = 6.0;
	auto aperture = 0.1;

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

	// Render
    time_t start = time(0);
    char time_str[] = "Calculating";
    double estimated_time = 0.0;
    std::ofstream out("image.ppm");

	out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height-1; j >= 0; --j)
	{
        double minutes_remaining = estimated_time * j / 60.0;

		std::clog << "\rEstimated time remaining: " << minutes_remaining << " minutes\t\t" <<
        std::flush;
		for (int i = 0; i < image_width; ++i)
		{
			color pixel_color(0, 0, 0);
			for(int s = 0; s < samples_per_pixel; ++s)
			{
				auto u = (i + random_double()) / (image_width-1);
				auto v = (j + random_double()) / (image_height-1);
				ray r = cam.get_ray(u, v);
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

