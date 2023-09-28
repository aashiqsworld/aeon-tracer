#include "Common.h"
#include <time.h>
#include "Camera.h"
#include "Color.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Material.h"
#include "Sphere.h"
#include "Mesh.h"
#include "MovingSphere.h"
#include "Texture.h"

#include <iostream>
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

HittableList random_scene() {
    HittableList world;

//    auto ground_material = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
//    world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

    auto checker = make_shared<CheckerTexture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, make_shared<Lambertian>(checker)));
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

HittableList random_scene_moving() {
    HittableList world;

//    auto ground_material = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
//    world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

    auto checker = make_shared<CheckerTexture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, make_shared<Lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<MovingSphere>(
                            center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int round4(int x)
{
    return x % 4 == 0 ? x : x - x % 4 + 4;
}

void random_spheres() {

    // test stuff START


    shared_ptr<Material> mat;
     // diffuse
    auto albedo = vec3(0.3, 0.9, 0.4);
    mat = make_shared<Lambertian>(albedo);
    auto m = make_shared<Mesh>(mat);
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
	const int samples_per_pixel = 10;
	const int max_depth = 50;

	// World
	auto world = random_scene_moving();
//	hittable_list world;
//    auto ground_material = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
//    world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));
//    world.add(m);

    // Camera
	point3 lookfrom(10,5,3);
	point3 lookat(0,0,0);
	vec3 vup(0,1,0);
	auto dist_to_focus = 6.0;
	auto aperture = 0.1;

	Camera cam(lookfrom, lookat, vup, 20, image_width, aspect_ratio, aperture, dist_to_focus, 0.0,
               1.0);

	// Render
    cam.render("image.ppm", 300, 150, world, 1);
}

void two_spheres()
{
    HittableList world;

    auto checker = make_shared<CheckerTexture>(0.8, color(.2, .3, .1), color(.9, .9, .9));

    world.add(make_shared<Sphere>(point3(0, -10, 0), 10, make_shared<Lambertian>(checker)));
    world.add(make_shared<Sphere>(point3(0, 10, 0), 10, make_shared<Lambertian>(checker)));

    float aspect_ratio = 16.0 / 9.0;
    int image_width = 600;
    int image_height = 150;

    int fvov = 20;
    point3 lookFrom = point3(13, 2, 3);
    point3 lookAt = point3(0,0,0);
    vec3 vUp = vec3(0,1,0);
    auto aperture = 0.1;
    auto focusDist = 6.0;

    Camera cam(lookFrom, lookAt, vUp, fvov, image_width, aspect_ratio, aperture, focusDist, 0.0,
               1.0);
    cam.render("two_spheres.ppm", image_width, static_cast<int>((double)image_width * (1.0/aspect_ratio)),
                                                        world, 10);

}

void earth() {
    auto earth_texture = make_shared<ImageTexture>("earthmap.jpg");
    auto earth_surface = make_shared<Lambertian>(earth_texture);
    auto globe = make_shared<Sphere>(point3(0,0,0), 2, earth_surface);

    auto aspect_ratio      = 16.0 / 9.0;
    auto image_width       = 400;
    auto samples_per_pixel = 100;
    auto max_depth         = 50;
    auto aperture = 0.1;
    auto focus_dist = 6.0;
    auto vfov     = 20;
    auto lookfrom = point3(0,0,12);
    auto lookat   = point3(0,0,0);
    auto vup      = vec3(0,1,0);

    Camera cam(lookfrom, lookat, vup, vfov, image_width, aspect_ratio, aperture, focus_dist);
    cam.render("earth.ppm", image_width, static_cast<int>((double)image_width * (1.0/aspect_ratio)),
               HittableList(globe), 5);
}

int main()
{
    switch(3) {
        case 1: random_spheres(); break;
        case 2: two_spheres(); break;
        case 3: earth(); break;
    }
}