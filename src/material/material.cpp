#include "material.h"

std::string Material::getId() const {
  return id;
}

Color SolidMaterial::shade(
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
