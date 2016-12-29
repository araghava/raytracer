#include "box.h"

Box::Box() :
  lowerBound(
    std::numeric_limits<float>::max(),
    std::numeric_limits<float>::max(),
    std::numeric_limits<float>::max()),
  upperBound(
    std::numeric_limits<float>::min(),
    std::numeric_limits<float>::min(),
    std::numeric_limits<float>::min()) {}

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
