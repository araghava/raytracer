#ifndef __RAY_H_
#define __RAY_H_

#include "../core/constants.h"
#include "../core/vector3.h"
#include "../objects/object.h"

class Object;
class Ray {
public:
  Ray() : origin(Vector()), direction(Vector()), remaining_casts(-1) {}
  Ray(Vector o, Vector d, int r) : origin(o), direction(d), remaining_casts(r) {
    direction.normalize();
    origin += direction * RAY_INTERSECTION_BIAS;
  }

  // Origin and direction that defines this ray.
  Vector origin;
  Vector direction;

  // How many more times can this ray be casted (reflected/refracted).
  int remaining_casts;
};

class Intersection {
public:
  Ray ray;
  Object *object = 0;
  Vector pt, nml;
};

#endif
