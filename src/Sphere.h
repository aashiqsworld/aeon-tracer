#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "vec3.h"

class Sphere : public Hittable
{
	public:
		Sphere() {}
		Sphere(point3 cen, double r, shared_ptr<Material> m)
			: center(cen), radius(r), mat_ptr(m) {
            auto rvec = vec3(radius, radius, radius);
            bbox = AABB(center - rvec, center + rvec);
        };

        AABB bounding_box() const override { return bbox; }
		virtual bool hit(const Ray& r, Interval ray_t, hit_record& rec) const override;

	public:
		point3 center;
		double radius;
		shared_ptr<Material> mat_ptr;
        AABB bbox;
};

bool Sphere::hit(const Ray& r, Interval ray_t, hit_record& rec) const
{
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;

	auto discriminant = half_b*half_b - a*c;
	if(discriminant < 0)
	{
		return false;
	}
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (!ray_t.surrounds(root))
	{
		root = (-half_b + sqrtd) / a;
		if(!ray_t.surrounds(root))
		{
			return false;
		}
	}
	
	rec.t = root;
	rec.p = r.at(rec.t);
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal); 
	rec.mat_ptr = mat_ptr;

	return true;
}


#endif