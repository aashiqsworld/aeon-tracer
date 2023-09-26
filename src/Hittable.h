#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "AABB.h"
#include "Common.h"

class Material;

struct hit_record
{
	point3 p;
	vec3 normal;
	shared_ptr<Material> mat_ptr;
	double t;
	bool front_face;

	inline void set_face_normal(const Ray& r, const vec3& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal :-outward_normal;
	}
};

class Hittable
{
	public:
		virtual bool hit(const Ray& r, Interval ray_t, hit_record& rec) const = 0;

        virtual AABB bounding_box() const = 0;
};

#endif