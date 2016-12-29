#include "bvh.h"

bool Bvh::intersect(const Ray& ray, Intersection& intersect) {
  float min_dist = std::numeric_limits<float>::max();
  bool found = false;

  // Check each object for intersection.
  for (int i = 0; i < (int)objects.size(); i++) {
    Intersection inter;
    if (objects[i]->intersect(ray, inter)) {
      found = true;
      float dist = (inter.pt - ray.origin).length2();

      // If this is closer, record this one.
      if (dist < min_dist) {
        min_dist = dist;
        intersect = inter;
      }
    }
  }
  return found;
}
