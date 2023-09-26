#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable
{
public:
    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() {
        objects.clear();
    }

    void add(shared_ptr<Hittable> object) {
        objects.push_back(object);
        bbox = AABB(bbox, object->bounding_box());
    }

    virtual bool hit(
            const Ray& r, Interval ray_t, hit_record& rec) const override;

    AABB bounding_box() const override {
        return bbox;
    }

public:
    std::vector<shared_ptr<Hittable>> objects;

private:
    AABB bbox;
};

bool HittableList::hit(const Ray& r, Interval ray_t, hit_record& rec) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = ray_t.max;

	for (const auto& object : objects)
	{
		if (object->hit(r, Interval(ray_t.min, closest_so_far), temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}	

	return hit_anything;
}

#endif