#ifndef __OBJOBJECT_H
#define __OBJOBJECT_H

#include <array>
#include <unordered_map>

#include "object.h"

#include "../acceleration/bvh.h"
#include "../core/tiny_obj_loader.h"

// Holds a triangular mesh and BVH to represent a .OBJ file.
class ObjObject : public Object {
 public:
  ObjObject(const Vector& center, const std::string& fn)
    : bvh(4), hasMaterials(false) {
    setCenter(center);
    load(fn);
  }
  virtual ~ObjObject() = default;

  virtual bool intersect(const Ray& ray, Intersection& intersection);

  virtual std::string getTextureOfFace(const size_t idx) {
    return !hasMaterials ? "null" : faceTextureMap[idx];
  }

  bool load(const std::string& fileName);

  virtual Color sampleTexture(const std::string& textureName);

 private:
  // Populates intersection struct if the ray hits the triangle and the intersection
  // is closer than other intersections on this object.
  bool rayTriangleIntersection(
    const Ray& ray,
    const std::vector<Vector>& triangle,
    const std::vector<Vector>& normals,
    Intersection& intersection);

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  // tinyobj::index_t holds indices into the attrib array above (which contains all the data).
  // Vertex data is packed in groups of 3 starting from the index, so to access the second vertex
  // of face 3:
  //   Vector secondVertex = Vector(
  //     attrib.vertices(faces[/* face three */ 3][/* second vertex */ 1] + 0), // x
  //     attrib.vertices(faces[3][1] + 1),  // y
  //     attrib.vertices(faces[3][1] + 2)); // z
  std::vector<std::array<tinyobj::index_t, 3>> faces;

  std::unordered_map<size_t, std::string> faceTextureMap;

  // Bounding volume hierarchy to avoid too many ray-triangle intersection tests.
  Bvh bvh;

  bool hasMaterials;
};

#endif
