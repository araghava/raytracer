#include "box.h"

#include "../core/util.h"

Box::Box() :
  lowerBound(
    std::numeric_limits<float>::max(),
    std::numeric_limits<float>::max(),
    std::numeric_limits<float>::max()),
  upperBound(
    -1 * std::numeric_limits<float>::max(),
    -1 * std::numeric_limits<float>::max(),
    -1 * std::numeric_limits<float>::max()) {}

void Box::extend(const Vector& pt) {
  lowerBound.x = std::min(pt.x, lowerBound.x);
  lowerBound.y = std::min(pt.y, lowerBound.y);
  lowerBound.z = std::min(pt.z, lowerBound.z);

  upperBound.x = std::max(pt.x, upperBound.x);
  upperBound.y = std::max(pt.y, upperBound.y);
  upperBound.z = std::max(pt.z, upperBound.z);
}

void Box::extend(const Box& box) {
  lowerBound.x = std::min(box.lowerBound.x, lowerBound.x);
  lowerBound.y = std::min(box.lowerBound.y, lowerBound.y);
  lowerBound.z = std::min(box.lowerBound.z, lowerBound.z);

  upperBound.x = std::max(box.upperBound.x, upperBound.x);
  upperBound.y = std::max(box.upperBound.y, upperBound.y);
  upperBound.z = std::max(box.upperBound.z, upperBound.z);
}

bool Box::intersect(const Ray& r) const {
  Vector dirInv = r.inverseDirection;
  float t1 = (lowerBound.x - r.origin.x) * dirInv.x;
  float t2 = (upperBound.x - r.origin.x) * dirInv.x;
  float t3 = (lowerBound.y - r.origin.y) * dirInv.y;
  float t4 = (upperBound.y - r.origin.y) * dirInv.y;
  float t5 = (lowerBound.z - r.origin.z) * dirInv.z;
  float t6 = (upperBound.z - r.origin.z) * dirInv.z;

  float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
  float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

  if (tmax < 0 || tmin > tmax) return false;
  return true;
}

Vector Box::getCenter() const {
  return lowerBound * 0.5f + upperBound * 0.5f;
}

Box Box::constructFromFace(
  const std::array<tinyobj::index_t, 3>& face,
  const tinyobj::attrib_t& attrib,
  const Vector& displace) {
  Box ret;
  ret.extend(UTILgetFaceVertex(face, attrib, 0) + displace);
  ret.extend(UTILgetFaceVertex(face, attrib, 1) + displace);
  ret.extend(UTILgetFaceVertex(face, attrib, 2) + displace);
  return ret;
}

int Box::getMaxDimension() const {
  float dx = upperBound.x - lowerBound.x;
  float dy = upperBound.y - lowerBound.y;
  float dz = upperBound.z - lowerBound.z;

  if (dx > dy && dx > dz) return 0;
  if (dy > dx && dy > dz) return 1;
  if (dz > dx && dz > dy) return 2;
  return 0;
}

