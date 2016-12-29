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

  return true;
}

bool ObjObject::intersect(const Ray& ray, Intersection& intersection) {
  for (size_t i = 0; i < shapes.size(); i++) {
    size_t index_offset = 0;

    for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
      size_t fnum = shapes[i].mesh.num_face_vertices[f];

      std::vector<Vector> face(3);
      std::vector<Vector> normals(3);
      for (size_t v = 0; v < fnum; v++) {
        tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
        face[v] = Vector(
          attrib.vertices[3 * idx.vertex_index + 0],
          attrib.vertices[3 * idx.vertex_index + 1],
          attrib.vertices[3 * idx.vertex_index + 2]) + center;
        normals[v] = Vector(
          attrib.normals[3 * idx.normal_index + 0],
          attrib.normals[3 * idx.normal_index + 1],
          attrib.normals[3 * idx.normal_index + 2]);
      }
      rayTriangleIntersection(ray, face, normals, intersection);
      index_offset += fnum;
    }
  }

  return intersection.finalized;
}

// Uses Moller-Trumbore ray-triangle intersection algorithm.
bool ObjObject::rayTriangleIntersection(
  const Ray& ray,
  const std::vector<Vector>& triangle,
  const std::vector<Vector>& normals,
  Intersection& intersection) {

  Vector a = triangle[0], b = triangle[1], c = triangle[2];
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
          intersection.object = this;
          intersection.pt = pt;
          intersection.nml = nml;
          intersection.ray = ray;
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

Color ObjObject::sampleTexture(const Vector& pt) const {
  return Color(1, 1, 1);
}
