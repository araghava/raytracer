#ifndef __SPHERE_H
#define __SPHERE_H

#include "object.h"
#include "../core/vector3.h"
#include "../trace/ray.h"

// Sphere object implementation.
class Sphere : public Object {
public:
             Sphere(const Vector c_, float rad)
                 : Object(c_),
                   radius(rad) {}
    virtual ~Sphere();

    float getRadius()
    { return radius; }

    // Virtual implementations.
    virtual bool intersect(const Ray &ray,
                           Intersection &intersection);
    virtual bool contains(const Vector &point);
private:
    float radius;
};

// Sphere area light implementation.
class SphereLight : public Light {
public:
             SphereLight(const Vector &p, Color c, float inten,
                         float rad)
                : Light(p, c, inten) { radius = rad; }
    virtual ~SphereLight() {}

    // We use a heuristic to decide how many samples to take on the sphere.
    // A reasonable one is min(9, max(surface_area/10, 128)).
    virtual int getNumSamples() const;

    // Uniform random sampling on the surface of the sphere.
    virtual void sample(Vector &point) const;

private:
    float radius;
};

#endif
