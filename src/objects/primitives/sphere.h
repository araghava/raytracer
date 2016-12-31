#ifndef __SPHERE_H
#define __SPHERE_H

#include "../object.h"
#include "../../core/vector3.h"
#include "../../trace/ray.h"

// Sphere object implementation.
class Sphere : public Object {
public:
  Sphere(const Vector c_, const float rad) : radius(rad) {
    setCenter(c_);
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
  SphereLight(const Vector& p, const Color c,
              const float inten, const float rad)
      : Light(p, c, inten) {
    radius = rad;
  }
  virtual ~SphereLight() {}

  // Take 8 * 8 random samples on the sphere.
  virtual int getNumSamples() const;

  // Uniform random sampling on the surface of the sphere.
  virtual void sample(Vector& point) const;

private:
  float radius;
};

#endif
