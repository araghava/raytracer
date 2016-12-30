#ifndef __BOX_H_
#define __BOX_H_

#include <algorithm>
#include <array>

#include <math.h>
#include <float.h>
#include "../core/tiny_obj_loader.h"
#include "../core/vector3.h"
#include "../trace/ray.h"

// Axis-aligned bounding box.
class Box {
 public:
  Box();

  // Extends box along axes
  void extend(const Vector& pt);
  void extend(const Box& box);

  //bool intersect(const Ray& r) const;
  bool intersect(const Ray& r) const;
  Vector getCenter() const;

  int getMaxDimension() const;

  static Box constructFromFace(
    const std::array<tinyobj::index_t, 3>& face,
    const tinyobj::attrib_t& attrib,
    const Vector& displace);

  Vector lowerBound;
  Vector upperBound;
};


#endif
