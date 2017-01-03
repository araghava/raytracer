#ifndef __RAY_H_
#define __RAY_H_

#include "../core/constants.h"
#include "../core/geometry.h"

class Ray {
 public:
  Ray() : origin(Vector()), direction(Vector()), remaining_casts(-1) {}
  Ray(Vector o, Vector d, int r) : origin(o), direction(d), remaining_casts(r) {
    direction.normalize();
    origin += direction * RAY_INTERSECTION_BIAS;

    // TODO: deal w/ division by zero
    inverseDirection = Vector();
    inverseDirection.x = 1.0 / direction.x;
    inverseDirection.y = 1.0 / direction.y;
    inverseDirection.z = 1.0 / direction.z;
  }

  // Origin and direction that defines this ray.
  Vector origin;
  Vector direction;
  Vector inverseDirection;

  // How many more times can this ray be casted (reflected/refracted).
  int remaining_casts;
};

struct Intersection {
  Intersection(bool _f = false) : finalized(_f) {}

  Ray ray;
  Vector pt, nml, texCoord;
  size_t objectId;

  bool finalized = false;
};

#endif
