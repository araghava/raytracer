#ifndef __OBJECT_H
#define __OBJECT_H

#include <memory>
#include <unordered_map>

#include "../texture/texture.h"
#include "../core/vector3.h"
#include "../core/color.h"
#include "../trace/ray.h"
#include "../acceleration/box.h"

class Object;

struct Intersection {
  Intersection(bool _f = false) : finalized(_f) {}

  Ray ray;
  Vector pt, nml;
  Object* object;
  std::string textureName;

  bool finalized = false;
};

// Abstract Object.
class Object {
public:
  Object() {
    textureMap["null"] = std::make_shared<NoopTexture>("null");
  }
  virtual ~Object() = default;

  // Methods to be overriden by subclasses.
  virtual bool intersect(const Ray& ray, Intersection& intersection) = 0;

  virtual Color sampleTexture(const std::string& textureName) {
    // TODO: get UV coordinate from pt...
    const double u = 0, v = 0;
    if (textureMap.count(textureName)) {
      return textureMap[textureName]->sample(u, v);
    }

    return Color(1, 1, 1);
  }

  void addTexture(const std::string& key, std::shared_ptr<Texture> t) {
    // Generic textures are applied to the whole thing, so get rid of the noop.
    textureMap.erase("null");
    textureMap[key] = t;
  }

  Vector getCenter() const {
    return center;
  }

  void setCenter(const Vector& pt) {
    center = pt;
  }

  virtual std::string getTextureOfFace(const size_t idx) {
    for (const auto& pair : textureMap) {
      return pair.first;
    }
    return "";
  }

protected:
  Vector center;
  Box bBox;

  std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;
};

// Abstract Light.
class Light {
public:
  Light(const Vector& p, Color c, float inten)
      : position(p), color(c), intensity(inten) {}
  virtual ~Light() {}

  // This gets the number of samples to use when trying to determine luminance
  // from a light. This is needed for area lights (to create soft shadows).
  virtual int getNumSamples() const = 0;

  // Uniform random sampling on the area of this light. Fills "point" with a
  // random
  // point on the surface of the light object.
  virtual void sample(Vector& point) const = 0;

  Vector position;
  Color color;
  float intensity;
};

class PointLight : public Light {
public:
  PointLight(const Vector& p, Color c, float inten) : Light(p, c, inten) {}
  virtual ~PointLight() = default;

  // A point light isn't an area, so we can only take one sample.
  virtual int getNumSamples() const { return 1; }

  virtual void sample(Vector& point) const { point = position; }
};

#endif
