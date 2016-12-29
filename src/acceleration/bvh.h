#ifndef __BVH_H_
#define __BVH_H_

#include <array>
#include <limits>
#include <memory>
#include <vector>

#include "../core/tiny_obj_loader.h"

// TODO: bounding volume hierarchy
class Bvh {
 public:
  Bvh() = default;

  void setMesh(
      std::vector<std::array<tinyobj::index_t, 3>>* const _faces,
      tinyobj::attrib_t* const _attrib) {
    faces = _faces;
    attrib = _attrib;
  }

  void buildTree();

 private:
  // These ptrs are owned by the object which this BVH is for.
  std::vector<std::array<tinyobj::index_t, 3>>* faces;
  tinyobj::attrib_t* attrib;
};

#endif
