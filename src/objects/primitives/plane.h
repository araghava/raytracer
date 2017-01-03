#ifndef __PLANE_H
#define __PLANE_H

#include "../object.h"

#include "../../core/geometry.h"
#include "../../core/constants.h"
#include "../../trace/ray.h"

#include <math.h>

// Plane object implementation. Useful to use as a ground plane.
class Plane : public Object {
public:
  // Defines a plane by three points.
  // Make sure that no two are collinear!
  Plane(const size_t id, const Vector& p1_, const Vector& p2_, const Vector& p3_)
      : Object(id), p1(p1_), p2(p2_), p3(p3_) {}
  virtual ~Plane() = default;

  // Virtual implementations.
  virtual bool intersect(const Ray& ray, Intersection& intersection) {
    Vector rp1 = transform.applyPoint(p1);
    Vector rp2 = transform.applyPoint(p2);
    Vector rp3 = transform.applyPoint(p3);
    Vector nml = ((rp2 - rp1).cross(rp3 - rp2)).normalize();
    if (ray.direction.dot(nml) > 0)
      nml = nml * -1;

    const float dp = ray.direction.dot(nml);

    // Ray and plane are parallel.
    if (fabs(dp) < TOLERANCE)
      return false;

    const float u = ((p1 - ray.origin).dot(nml)) / dp;

    // The intersection point is behind where we fired the ray.
    if (u < 0)
      return false;

    const Vector pt = ray.origin + ray.direction * u;
    const float dist = (pt - ray.origin).length2();

    // The intersection is too far for it to count.
    if (dist >= RAY_FAR_DISTANCE)
      return false;

    intersection.objectId = objectId;
    intersection.ray = ray;
    intersection.pt = ray.origin + ray.direction * u;
    intersection.nml = nml;
    intersection.finalized = true;

    return true;
  }

private:
  float radius;
  Vector p1, p2, p3;
};

#endif
