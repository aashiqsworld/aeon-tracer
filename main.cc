#include "common.h"

#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include <iostream>
color ray_color(const ray& r, const hittable& world, int depth)
{
	hit_record rec;

	// if we've exceeded the ray bounce limit, no more light is gathered
	if(depth <= 0)
		return color(0, 0, 0);

	if(world.hit(r, 0.001, infinity, rec))
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

int round4(int x)
{
    return x % 4 == 0 ? x : x - x % 4 + 4;
}

// renders a ray in the scene
int main() {


	// Image
    const char *filename = "image.bmp";
	const auto aspect_ratio = 3.0 / 2.0;
	const int image_width = 600;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 6;
	const int max_depth = 50;

    // bmp vars
    int padded_width = round4(image_width * 3);
    int bitmap_size = image_height * padded_width * 3;
    char *bitmap = (char *) malloc(bitmap_size * sizeof(char));
    for (int i = 0; i < bitmap_size; i++) bitmap[i] = 0;




	// World
	auto world = random_scene();

	// Camera

	point3 lookfrom(13,2,3);
	point3 lookat(0,0,0);
	vec3 vup(0,1,0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);



	// Render
//	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height-1; j >= 0; --j)
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
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
//			write_color(std::cout, pixel_color, samples_per_pixel);
            int index = j * padded_width + i * 3;
            bitmap[index  ] = pixel_color.r_char(samples_per_pixel);
            bitmap[index+1] = pixel_color.g_char(samples_per_pixel);
            bitmap[index+2] = pixel_color.b_char(samples_per_pixel);
		}
	}

    char tag[] = { 'B', 'M' };
    int header[] = {
            0, 0, 0x36, 0x28, image_width, image_height, 0x180001,
            0, 0, 0x002e23, 0x002e23, 0, 0
    };
    header[0] = sizeof(tag) + sizeof(header) + bitmap_size;
    FILE *fp = fopen(filename, "w+");
    fwrite(&tag, sizeof(tag), 1, fp);
    fwrite(&header, sizeof(header), 1, fp);
    fwrite(bitmap, bitmap_size * sizeof(char), 1, fp);
    fclose(fp);
    free(bitmap);

	std::cerr << "\nDone.\n";
}

