#ifndef __BVH_H_
#define __BVH_H_

#include <limits>
#include <memory>
#include <vector>

#include "../objects/object.h"

// TODO: bounding volume hierarchy
class Bvh {
 public:
  Bvh() = default;

  void addObject(const std::shared_ptr<Object> obj) {
    objects.push_back(obj);
  }

  bool intersect(const Ray& ray, Intersection& intersect);

 private:
  std::vector<std::shared_ptr<Object>> objects;
};

#endif
