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

bool Box::isInside(const Vector& p) const {
  return !((lowerBound.x > p.x || upperBound.x < p.x)
         || (lowerBound.y > p.y || upperBound.y < p.y)
         || (lowerBound.z > p.z || upperBound.z < p.z));
}

bool Box::intersect(const Ray& r) const {
  if (isInside(r.origin)) {
    return true;
  }

  float minX = r.direction.x == 0.0 ?
    -1 * std::numeric_limits<float>::max() : (lowerBound.x - r.origin.x) / r.direction.x;
  float maxX = r.direction.x == 0.0 ?
    std::numeric_limits<float>::max() : (upperBound.x - r.origin.x) / r.direction.x;
  float minY = r.direction.y == 0.0 ?
    -1 * std::numeric_limits<float>::max() : (lowerBound.y - r.origin.y) / r.direction.y;
  float maxY = r.direction.y == 0.0 ?
    std::numeric_limits<float>::max() : (upperBound.y - r.origin.y) / r.direction.y;
  if(minX > maxX) {
    std::swap(minX, maxX);
  }
  if(minY > maxY) {
    std::swap(minY, maxY);
  }

  if (!(minX <= maxY && minY <= maxX)) {
    return false;
  }

  float minT = std::max(minX, minY);
  float maxT = std::min(maxX, maxY);

  float minZ = r.direction.z == 0.0 ?
    -1 * std::numeric_limits<float>::max() : (lowerBound.z - r.origin.z) / r.direction.z;
  float maxZ = r.direction.z == 0.0 ?
    std::numeric_limits<float>::max() : (upperBound.z - r.origin.z) / r.direction.z;
  if(minZ > maxZ) {
    std::swap(minZ, maxZ);
  }

  if(!(minT <= maxZ && minZ <= maxT)) {
    return false;
  }

  minT = std::max(minT, minZ);
  maxT = std::min(maxT, maxZ);

  // make sure ray enters before leaving and all hits are along +ve direction
  if(minT <= maxT && minT > 0.0){
    return true;
  }

  return false;
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

