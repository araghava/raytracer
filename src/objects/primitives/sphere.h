#ifndef __SPHERE_H
#define __SPHERE_H

#include "../light.h"
#include "../object.h"
#include "../../core/vector3.h"
#include "../../trace/ray.h"

// Sphere object implementation.
class Sphere : public Object {
public:
  Sphere(const size_t id, const float rad)
    : Object(id), radius(rad) {
  }
  virtual ~Sphere() = default;

  float getRadius() const {
    return radius;
  }

  // Virtual implementations.
  virtual bool intersect(const Ray& ray, Intersection& intersection);

private:
  float radius;
};

// Sphere area light implementation.
class SphereLight : public Light {
public:
  SphereLight(const Color c, const float inten, const float rad)
      : Light(c, inten) {
    radius = rad;
  }
  virtual ~SphereLight() {}

  // Take 8 * 8 random samples on the sphere.
  virtual int getNumSamples() const;

  // Uniform random sampling on the surface of the sphere.
  virtual Vector sample() const;

private:
  float radius;
};

#endif
