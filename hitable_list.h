#ifndef HITABLELIST_H_
#define HITABLELIST_H_

#include "hitable.h"

class hitable_list : public hitable {
public:
    hitable_list() {}
    hitable_list(hitable **l, int n)
    {
        list = l;
        list_size = n;
    }
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
    virtual bool bounding_box(float t0, float t1, aabb &box) const;
    virtual float pdf_value(const vec3 &origin, const vec3 &v) const;
    virtual vec3 random(const vec3 &origin);
    hitable **list;
    int list_size;
};

bool hitable_list::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size; i++) {
        if (list[i]->hit(r, t_min, t_max, temp_rec)) {
            hit_anything = true;
            if (closest_so_far > temp_rec.t) {
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
    }
    return hit_anything;
}

bool hitable_list::bounding_box(float t0, float t1, aabb &box) const {
    if (list_size < 1)
        return false;
    aabb temp_box;
    bool first_true = list[0]->bounding_box(t0, t1, temp_box);
    if (!first_true)
        return false;
    else
        box = temp_box;
    for (int i = 1; i < list_size; i++) {
        if (list[0]->bounding_box(t0, t1, temp_box)) {
            box = surrounding_box(box, temp_box);
        }
        else
            return false;
    }
    return true;
}

float hitable_list::pdf_value(const vec3 &origin, const vec3 &v) const {
    float weight = 1.0 / list_size;
    float sum = 0;
    for(int i=0;i<list_size;i++){
        sum += weight*list[i]->pdf_value(origin, v);
    }
    return sum;
}

vec3 hitable_list::random(const vec3 &origin) {
    int index = int(drand() * list_size);
    if(index == list_size) index--;
    return list[index]->random(origin);
}


#endif
