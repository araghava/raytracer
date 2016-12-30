#include "bvh.h"
#include <iostream>
#include "../core/util.h"

BvhFace::BvhFace(const int idx, const Box& box)
  : faceIdx(idx), bBox(box) {
    centroid = bBox.getCenter();
}

BvhNode::BvhNode()
  : leftChild(nullptr), rightChild(nullptr), splitAxis(-1), numFaces(0) {
}

void BvhNode::initLeaf(size_t start, size_t nf, const Box& box) {
  bBox = box;
  faceOffset = start;
  numFaces = nf;
}

void BvhNode::initParent(
    int axis,
    std::shared_ptr<BvhNode> left,
    std::shared_ptr<BvhNode> right) {
  splitAxis = axis;
  leftChild = left;
  rightChild = right;

  bBox.extend(left->bBox);
  bBox.extend(right->bBox);
}

void Bvh::buildTree() {
  std::vector<BvhFace> primitives;
  primitives.reserve(faces->size());

  for (size_t i = 0; i < faces->size(); i++) {
    primitives.push_back(BvhFace(i,
      Box::constructFromFace((*faces)[i], *attrib, objectPtr->getCenter())));
  }

  // This function modifies the primitives array such that leaf BvhNodes will occupy
  // contiguous ranges of primitives in the array. The modified ordering is defined
  // by this->order.
  root = buildTreeRecursive(0, primitives.size(), primitives);
}

std::shared_ptr<BvhNode> Bvh::buildTreeRecursive(
  size_t start, size_t end, std::vector<BvhFace>& primitives) {
  // TODO: static memory arena to make these allocs faster
  auto node = std::make_shared<BvhNode>();
  Box allBounds;
  for (size_t i = start; i < end; i++) {
    allBounds.extend(primitives[i].bBox);
  }

  size_t num = end - start;
  if (num <= maxFacesInNode) {
    size_t offset = order.size();
    for (size_t i = start; i < end; i++) {
      order.push_back(primitives[i].faceIdx);
    }
    node->initLeaf(offset, num, allBounds);
  } else {
    int splitDim;
    size_t mid = partitionBestSplit(start, end, primitives, &splitDim);
    if (mid <= start || mid >= end) {
      size_t offset = order.size();
      for (size_t i = start; i < end; i++) {
        order.push_back(primitives[i].faceIdx);
      }
      node->initLeaf(offset, num, allBounds);
    } else {
      auto leftTree = buildTreeRecursive(start, mid, primitives);
      auto rightTree = buildTreeRecursive(mid, end, primitives);
      node->initParent(splitDim, leftTree, rightTree);
    }
  }
  return node;
}

size_t Bvh::partitionBestSplit(
  size_t start, size_t end, std::vector<BvhFace>& primitives, int* splitDim) {
  Box centroidBox;
  for (size_t i = start; i < end; i++) {
    centroidBox.extend(primitives[i].centroid);
  }
  auto dim = centroidBox.getMaxDimension();
  if (centroidBox.upperBound[dim] == centroidBox.lowerBound[dim]) {
    for (int i = 0; i < 3; i++) {
      if (i != dim && centroidBox.upperBound[i] != centroidBox.lowerBound[i]) {
        dim = i;
        break;
      }
    }
  }

  if (centroidBox.upperBound[dim] == centroidBox.lowerBound[dim]) {
    return -1;
  }

  float midPoint = 0.5f * (centroidBox.lowerBound[dim] + centroidBox.upperBound[dim]);
  size_t mid = std::partition(primitives.begin() + start, primitives.begin() + end,
    [midPoint, dim](const BvhFace& face) {
      if (dim == 0) return face.centroid.x < midPoint;
      else if (dim == 1) return face.centroid.y < midPoint;
      else return face.centroid.z < midPoint;
    }) - primitives.begin();
  (*splitDim) = dim;

  return mid;
}

bool Bvh::intersect(const Ray& ray, Intersection& intersection) {
  return intersectRecursive(root, ray, intersection);
}

bool Bvh::intersectRecursive(
  std::shared_ptr<BvhNode> node, const Ray& ray, Intersection& intersection) {
  if (!node || !node->bBox.intersect(ray)) return false;

  if (node->numFaces > 0) {
    // Leaf node, check each primitive that belongs to the leaf.
    for (size_t i = 0; i < node->numFaces; i++) {
      rayTriangleIntersection(ray, order[node->faceOffset + i], intersection);
    }
  } else {
    intersectRecursive(node->leftChild, ray, intersection);
    intersectRecursive(node->rightChild, ray, intersection);
  }
  return intersection.finalized;
}

// Uses Moller-Trumbore ray-triangle intersection algorithm.
bool Bvh::rayTriangleIntersection(
  const Ray& ray, const size_t faceIdx, Intersection& intersection) {
  Vector a = UTILgetFaceVertex((*faces)[faceIdx], *attrib, 0) + objectPtr->getCenter();
  Vector b = UTILgetFaceVertex((*faces)[faceIdx], *attrib, 1) + objectPtr->getCenter();
  Vector c = UTILgetFaceVertex((*faces)[faceIdx], *attrib, 2) + objectPtr->getCenter();

  std::vector<Vector> normals = {
    UTILgetFaceNormal((*faces)[faceIdx], *attrib, 0),
    UTILgetFaceNormal((*faces)[faceIdx], *attrib, 1),
    UTILgetFaceNormal((*faces)[faceIdx], *attrib, 2)
  };

  Vector e1 = b - a, e2 = c - a;
  Vector D = ray.direction;
  D.normalize();
  Vector P = D.cross(e2);

  float determ = e1.dot(P);
  bool back = false;

  // back-face hits
  if (determ < -TOLERANCE) {
    back = true;
    e1 = c - a;
    e2 = b - a;
    P = D.cross(e2);
    determ = e1.dot(P);
  }

  if (determ > TOLERANCE) {
    Vector T = ray.origin - a;
    float u = T.dot(P);

    if (u > -TOLERANCE && u < determ * (1.0 + TOLERANCE)) {
      Vector Q = T.cross(e1);
      float v = D.dot(Q);

      if (v > -TOLERANCE && v + u < determ * (1.0 + TOLERANCE)) {
        u /= determ;
        v /= determ;
        float t = (e2.dot(Q)) / determ;

        // barycentric coordinate vector on the face
        Vector bc = Vector(1.0 - u - v, u, v);
        Vector nml = UTILinterpolateFace(normals, bc);
        Vector pt = ray.origin + (D * t);

        if (t > TOLERANCE && (!intersection.finalized
            || (pt - ray.origin).length2() < (intersection.pt - ray.origin).length2())) {
          intersection.object = objectPtr;
          intersection.pt = pt;
          intersection.nml = nml;
          intersection.ray = ray;
          intersection.textureName = objectPtr->getTextureOfFace(faceIdx);
          intersection.finalized = true;

          if (back) {
            intersection.nml = intersection.nml * -1;
          }
          return true;
        }
      }
    }
  }
  return false;
}
