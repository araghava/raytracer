#define TINYOBJLOADER_IMPLEMENTATION
#include "objobject.h"

#include <algorithm>
#include <iostream>

#include "../core/util.h"

bool ObjObject::load(const std::string& fileName) {
  std::cout << "Loading " << fileName << std::endl;

  std::string err;
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(),
                              "../models/", /*triangulate=*/true);
  if (!err.empty()) {
    std::cerr << err << std::endl;
  }

  if (!ret) {
    printf("Failed to load/parse .obj.\n");
    return false;
  }

  // compute bounding box and set face attribute indexes
  for (size_t i = 0; i < shapes.size(); i++) {
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
      size_t fnum = shapes[i].mesh.num_face_vertices[f];
      std::array<tinyobj::index_t, 3> face;

      for (size_t v = 0; v < fnum; v++) {
        tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
        bBox.extend(Vector(
          attrib.vertices[3 * idx.vertex_index + 0],
          attrib.vertices[3 * idx.vertex_index + 1],
          attrib.vertices[3 * idx.vertex_index + 2]) + transform.translation);
        face[v] = idx;
      }

      faces.push_back(std::move(face));
      index_offset += fnum;
    }
  }

  bvh.setMesh(&faces, &attrib, this);
  bvh.buildTree();

  return true;
}

bool ObjObject::intersect(const Ray& ray, Intersection& intersection) {
  if (!bBox.intersect(ray)) {
    return false;
  }

  return bvh.intersect(ray, intersection);
}
