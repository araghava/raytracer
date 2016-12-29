#ifndef __BOX_H_
#define __BOX_H_

#include <algorithm>

#include "../core/vector3.h"
#include "../trace/ray.h"

// Axis-aligned bounding box.
class Box {
 public:
  Box();

  // Extends box along axes
  void extend(const Vector& pt);
  void extend(const Box& box);

  bool intersect(const Ray& r) const;

 private:
  Vector lowerBound;
  Vector upperBound;
};

#endif
