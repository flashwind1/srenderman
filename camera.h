#ifndef CAMERA_H_
#define CAMERA_H_
#include <math.h>

#include "ray.h"

vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2 * vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0) - vec3(1.0, 1.0, 0.0);

    } while (dot(p, p) > 1.0);
    return p;
}

class camera {
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist,
    float t0, float t1)
    { // vfov is top to bottom in degree
        time0 = t0;
        time1 = t1;
        lens_radius = aperture / 2;
        float theta = vfov * 3.14159265358979323846 / 180.0;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }

    ray get_ray(float s, float t) {
        //return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin); }
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        float time = time0 + (float)rand()/(float)RAND_MAX * (time1 - time0);
        return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset, time);
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float time0, time1; // new variables for shutter open/close times
    float lens_radius;
};

#endif