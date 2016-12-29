#ifndef __OBJOBJECT_H
#define __OBJOBJECT_H

#include "object.h"

#include "../core/tiny_obj_loader.h"

class ObjObject : public Object {
 public:
  ObjObject(const Vector& center, const std::string& fn) {
    setCenter(center);
    load(fn);
  }
  virtual ~ObjObject() = default;

  // Currently checks every triangle, but should be implemented with an acceleration
  // data structure (BVH/octree).
  virtual bool intersect(const Ray& ray, Intersection& intersection);
  virtual bool contains(const Vector& point) const {
    return true;
  }

  bool load(const std::string& fileName);

  virtual Color sampleTexture(const Vector& pt) const;

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
};

#endif
