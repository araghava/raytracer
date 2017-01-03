#ifndef __OBJECT_H
#define __OBJECT_H

#include <memory>
#include <unordered_map>

#include "../core/matrix.h"
#include "../core/geometry.h"
#include "../core/color.h"
#include "../trace/ray.h"
#include "../acceleration/box.h"
#include "../material/material.h"

// Abstract Object.
class Object {
public:
  Object(const size_t objId)
    : material(std::make_shared<NoopMaterial>("null")),
      objectId(objId) {
  }
  virtual ~Object() = default;

  // Methods to be overriden by subclasses.
  virtual bool intersect(const Ray& ray, Intersection& intersection) = 0;
  virtual bool load() {
    return true;
  }

  std::shared_ptr<Material> getMaterial() const {
    return material;
  }

  void setMaterial(std::shared_ptr<Material>& t) {
    material = t;
  }

  void setTransform(const Transform& t) {
    transform = t;
  }

  Transform getTransform() const {
    return transform;
  }

  void setCenter(const Vector& pt) {
    transform.translation = pt;
  }

  size_t getId() const {
    return objectId;
  }

protected:
  Box bBox;
  std::shared_ptr<Material> material;
  Transform transform;
  const size_t objectId;
};

#endif
