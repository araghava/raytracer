#ifndef __OBJOBJECT_H
#define __OBJOBJECT_H

#include "object.h"

class ObjObject : public Object {
 public:
  ObjObject(const Vector& center, const std::string& fn) {
    setCenter(center);
    load(fn);
  }
  virtual ~ObjObject() = default;

  virtual bool intersect(const Ray& ray, Intersection& intersection) {
    return false;
  }
  virtual bool contains(const Vector& point) const {
    return true;
  }

  bool load(const std::string& fileName);

  virtual Color sampleTexture(const Vector& pt) const;
};

#endif
