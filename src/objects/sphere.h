#ifndef __SPHERE_H
#define __SPHERE_H

#include "object.h"
#include "../core/vector3.h"
#include "../trace/ray.h"

// Sphere object implementation.
class Sphere : public Object {
public:
             Sphere(const Vector c_, float rad)
                 : radius(rad) { setCenter(c_); }
    virtual ~Sphere();

    float getRadius()
    { return radius; }

    // Virtual implementations.
    virtual bool intersect(const Ray &ray, Intersection &intersection);
    virtual bool contains(const Vector &point);
private:
    float radius;
};

#endif
