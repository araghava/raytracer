#ifndef __PLANE_H
#define __PLANE_H

#include "object.h"

#include "../core/vector3.h"
#include "../core/constants.h"
#include "../trace/ray.h"

#include <math.h>

// Plane object implementation. Useful to use as a ground plane.
class Plane : public Object {
public:
  // Defines a plane by three points.
  // Make sure that no two are collinear!
  Plane(const Vector &p1_, const Vector &p2_, const Vector &p3_)
      : Object(Vector(0, 0, 0)), p1(p1_), p2(p2_), p3(p3_) {}
  virtual ~Plane() {}

  // Virtual implementations.
  virtual bool intersect(const Ray &ray, Intersection &intersection) {
    Vector nml = ((p2 - p1).cross(p3 - p2)).normalize();
    if (ray.direction.dot(nml) > 0)
      nml = nml * -1;

    float dp = ray.direction.dot(nml);

    // Ray and plane are parallel.
    if (fabs(dp) < TOLERANCE)
      return false;

    float u = ((p1 - ray.origin).dot(nml)) / dp;

    // The intersection point is behind where we fired the ray.
    if (u < 0)
      return false;

    Vector pt = ray.origin + ray.direction * u;
    float dist = (pt - ray.origin).length2();

    // The intersection is too far for it to count.
    if (dist >= RAY_FAR_DISTANCE)
      return false;

    intersection.object = this;
    intersection.ray = ray;
    intersection.pt = ray.origin + ray.direction * u;
    intersection.nml = nml;
    return true;
  }

  virtual bool contains(const Vector &point) {
    Vector nml = ((p2 - p1).cross(p3 - p2)).normalize();
    return fabs((point - p1).dot(nml)) < TOLERANCE;
  }

private:
  float radius;
  Vector p1, p2, p3;
};

#endif
