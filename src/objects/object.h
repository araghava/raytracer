#ifndef __OBJECT_H
#define __OBJECT_H

#include <memory>
#include <unordered_map>

#include "../core/matrix.h"
#include "../core/vector3.h"
#include "../core/color.h"
#include "../trace/ray.h"
#include "../acceleration/box.h"
#include "../texture/texture.h"

// Abstract Object.
class Object {
public:
  Object(const size_t objId)
    : texture(std::make_shared<NoopTexture>("null")),
      objectId(objId) {
  }
  virtual ~Object() = default;

  // Methods to be overriden by subclasses.
  virtual bool intersect(const Ray& ray, Intersection& intersection) = 0;
  virtual bool load() {
    return true;
  }

  virtual Color sampleTexture(const Vector& pt) {
    // TODO: get UV coordinate from pt...
    const double u = 0, v = 0;
    return texture->sample(u, v);
  }

  std::shared_ptr<Texture> getTexture() const {
    return texture;
  }

  void setTexture(std::shared_ptr<Texture>& t) {
    texture = t;
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
  std::shared_ptr<Texture> texture;
  Transform transform;
  const size_t objectId;
};

#endif
