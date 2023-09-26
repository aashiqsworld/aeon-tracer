#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "Common.h"
#include "Hittable.h"

class MovingSphere : public Hittable {
public:
    MovingSphere() {}
    MovingSphere (
            point3 cen0, point3 cen1, double _time0, double _time1, double r,
            shared_ptr<Material> m)
            : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
    {
        auto rvec = vec3(radius, radius, radius);
        AABB box1(center0 - rvec, center0 + rvec);
        AABB box2(center1 - rvec, center1 + rvec);
        bbox = AABB(box1, box2);
    };

    virtual bool hit(
            const Ray& r, Interval ray_t, hit_record& rec) const override;
    virtual AABB bounding_box() const override;


    point3 center(double time) const;

public:
    point3 center0, center1;
    double time0, time1;
    double radius;
    shared_ptr<Material> mat_ptr;
    AABB bbox;
};

point3 MovingSphere::center(double time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool MovingSphere::hit(const Ray& r, Interval ray_t, hit_record& rec) const {
    vec3 oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < ray_t.min || ray_t.max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < ray_t.min || ray_t.max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    auto outward_normal = (rec.p - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

AABB MovingSphere::bounding_box() const
{
    return bbox;

}

#endif