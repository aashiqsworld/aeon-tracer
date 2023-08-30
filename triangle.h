//
// Created by Aashiq Shaikh on 8/30/23.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

using namespace std;

class triangle : public hittable
{
public:
    triangle() = default;
    triangle(vec3 v1, vec3 v2, vec3 v3) : vertices{v1, v2, v3} {}
    void setVertices(vec3 v1, vec3 v2, vec3 v3) {
        vertices[0] = v1;
        vertices[1] = v2;
        vertices[2] = v3;
    }
    void setNormal(vec3 n) { normal = n; }
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
public:
    vec3 vertices[3];
    vec3 normal; // normal vector
};

bool triangle::hit(const ray& r, interval ray_t, hit_record& rec) const
{
    vec3 vertex0 = vertices[0];
    vec3 vertex1 = vertices[1];
    vec3 vertex2 = vertices[2];
    vec3 edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = cross(r.direction(),edge2);
    a = dot(edge1,h);

    if(a > -EPSILON && a < EPSILON)
        return false; // This ray is parallel to this triangle

    f = 1.0 / a;
    s = r.origin() - vertex0;
    u = f * dot(s,h);

    if(u < 0.0 || u > 1.0)
        return false;

    q = cross(s, edge1);
    v = f * dot(r.direction(),q);

    if(v < 0.0 || u + v > 1.0)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line

    float t = f * dot(edge2,q);

    if(t > EPSILON)
    {
        rec.p = r.origin() + r.direction() * t;
        return true;
    }
    else // This means there is a line intersection but not a ray intersection
        return false;
}

inline std::ostream& operator<<(std::ostream &out, const triangle &t)
{
    return out << "Triangle: " << endl <<
    "v1: (" << t.vertices[0] << ")  (" << t.vertices[1] << ")  (" << t.vertices[2] << ")" <<
    "normal: (" << t.normal << ")";

}


#endif
