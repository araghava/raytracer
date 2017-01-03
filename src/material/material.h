#ifndef __MATERIAL_H
#define __MATERIAL_H

#include <memory>
#include <string>
#include <vector>

#include "../core/geometry.h"
#include "../core/color.h"
#include "../objects/light.h"
#include "../trace/ray.h"

// Material applicable to objects
class Material {
public:
  Material(const std::string& s) : id(s) {}
  virtual ~Material() = default;

  virtual Color shade(
    const Intersection& inter,
    std::shared_ptr<Light>& light,
    const Vector& lightSamplePos) const = 0;

  std::string getId() const;

private:
  std::string id;
};

// Solid color material.
class SolidMaterial : public Material {
public:
  SolidMaterial(const std::string id, const Color c)
    : Material(id), color(c) {}

  virtual Color shade(
      const Intersection& inter,
      std::shared_ptr<Light>& light,
      const Vector& lightSamplePos) const;

private:
  Color color;
};

class NoopMaterial : public SolidMaterial {
public:
  NoopMaterial(const std::string id) : SolidMaterial(id, Color(1, 1, 1)) {}
};

#endif
