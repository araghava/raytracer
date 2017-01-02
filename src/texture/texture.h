#ifndef __TEXTURE_H
#define __TEXTURE_H

#include <memory>
#include <string>
#include <vector>

#include "../core/vector3.h"
#include "../core/color.h"
#include "../objects/light.h"
#include "../trace/ray.h"

// Texture applicable to objects
class Texture {
public:
  Texture(const std::string& s) : id(s) {}
  virtual ~Texture() = default;

  virtual Color sample(const double i, const double j) const = 0;
  virtual Color shade(
    const Intersection& inter,
    std::shared_ptr<Light>& light,
    const Vector& lightSamplePos) const = 0;

  std::string getId() const;

private:
  std::string id;
};

// Solid color texture.
class SolidTexture : public Texture {
public:
  SolidTexture(const std::string id, const Color c)
    : Texture(id), color(c) {}

  virtual Color sample(const double i, const double j) const {
    return color;
  }

  virtual Color shade(
      const Intersection& inter,
      std::shared_ptr<Light>& light,
      const Vector& lightSamplePos) const {
    // The amount this light contributes is proportional to the dot
    // product of the relative light position and surface normal.
    Vector pos_off = (lightSamplePos - inter.pt).normalize();
    float diff_contrib = inter.nml.dot(pos_off);
    if (diff_contrib > 0.0) {
      return color * light->color * light->intensity * diff_contrib;
    }
    return Color(0, 0, 0);
  }

private:
  Color color;
};

class NoopTexture : public SolidTexture {
public:
  NoopTexture(const std::string id) : SolidTexture(id, Color(1, 1, 1)) {}
};

#endif
