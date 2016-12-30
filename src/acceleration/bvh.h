#ifndef __BVH_H_
#define __BVH_H_

#include <array>
#include <limits>
#include <memory>
#include <vector>

#include "box.h"
#include "../core/tiny_obj_loader.h"
#include "../trace/ray.h"
#include "../objects/object.h"

struct BvhFace;
struct BvhNode;

// BVH to speed up triangular mesh rendering. Each mesh has its own BVH.
class Bvh {
 public:
  Bvh(const int mp) : maxFacesInNode(mp) {}

  void setMesh(
      std::vector<std::array<tinyobj::index_t, 3>>* const _faces,
      tinyobj::attrib_t* const _attrib,
      Object* _oPtr) {
    faces = _faces;
    attrib = _attrib;
    objectPtr = _oPtr;
    order.reserve(faces->size());
  }

  // Builds a binary tree of std::shared_ptr<BvhNode> and sets the root.
  // Since each leaf BvhNode stores an offset and the number of primitives it contains, we can
  // record the primitives that correspond to each BvhNode with only these two values.
  void buildTree();

  bool intersect(const Ray& ray, Intersection& intersection);

 private:
  std::shared_ptr<BvhNode> buildTreeRecursive(
    size_t start, size_t end, std::vector<BvhFace>& primitives);

  // We want the BVH to be spatially balanced (left and right children should be in charge
  // of geometry which are relatively far away). This function partitions primitives[start:end-1]
  // (corresponding to a BvhNode) along the axis of greatest variation (largest axis of BBox).
  // Returns the midpoint of the partition.
  size_t partitionBestSplit(
    size_t start, size_t end, std::vector<BvhFace>& primitives, int* splitDim);

  // Recursively traverses the BVH to find leaf nodes whose
  // primitives are candidates for intersections.
  bool intersectRecursive(
    std::shared_ptr<BvhNode> node, const Ray& ray, Intersection& intersection);

  // Populates intersection struct if the ray hits the face and the intersection
  // is closer than other intersections on this object.
  bool rayTriangleIntersection(
    const Ray& ray, const size_t faceIdx, Intersection& intersection);

  // These ptrs are owned by the object which this BVH is for.
  std::vector<std::array<tinyobj::index_t, 3>>* faces;
  tinyobj::attrib_t* attrib;

  // Max. number of faces per leaf node.
  const int maxFacesInNode;

  // After tree construction, we want the faces for each leaf to occupy
  // continuous ranges (so we can keep track of what faces belong to what
  // leaves with only two offsets). This defines the ordering into "faces" above:
  std::vector<size_t> order;

  std::shared_ptr<BvhNode> root;

  Object* objectPtr;
};

// Struct representing a triangular face in the BVH.
struct BvhFace {
  BvhFace(int idx, const Box& box);
  int faceIdx;
  Box bBox;
  Vector centroid;
};

struct BvhNode {
  BvhNode();

  // Creates this node as a leaf or parent.
  void initLeaf(size_t start, size_t numFaces, const Box& box);
  void initParent(int axis, std::shared_ptr<BvhNode> left, std::shared_ptr<BvhNode> right);

  std::shared_ptr<BvhNode> leftChild;
  std::shared_ptr<BvhNode> rightChild;
  Box bBox;

  // Which axis the node is split on.
  int splitAxis;

  // The faces that this node contains (if it's a leaf node) consists of:
  //   faces[order[faceOffset + 0]] ... faces[order[faceOffset + numFaces - 1]]
  size_t faceOffset;
  size_t numFaces;
};

#endif
