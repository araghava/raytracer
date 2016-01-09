#ifndef __OBJECT_H
#define __OBJECT_H

#include <memory>

#include "texture.h"
#include "../core/vector3.h"
#include "../core/color.h"
#include "../trace/ray.h"

struct Intersection;
class Ray;

// Abstract Object.
class Object {
public:
  Object(const Vector &c) : center(c), texture(0) {}
  virtual ~Object() {}

  const Vector& getCenter() { return center; }

  // Methods to be overriden by subclasses.
  virtual bool intersect(const Ray &ray, Intersection &intersection) = 0;
  virtual bool contains(const Vector &point) = 0;

  Color sampleTexture(const Vector &pt) {
    // TODO: get UV coordinate from pt...
    const double u = 0, v = 0;
    if (texture) {
      return texture->sample(u, v);
    }

    return Color(0,0,0);
  }

  void setTexture(std::shared_ptr<Texture> t) {
    texture = t;
  }

private:
  Vector center;
  std::shared_ptr<Texture> texture;
};

// Abstract Light.
class Light {
public:
  Light(const Vector &p, Color c, float inten)
      : position(p), color(c), intensity(inten) {}
  virtual ~Light() {}

  // This gets the number of samples to use when trying to determine luminance
  // from a light. This is needed for area lights (to create soft shadows).
  virtual int getNumSamples() const = 0;

  // Uniform random sampling on the area of this light. Fills "point" with a
  // random
  // point on the surface of the light object.
  virtual void sample(Vector &point) const = 0;

  Vector position;
  Color color;
  float intensity;
};

class PointLight : public Light {
public:
  PointLight(const Vector &p, Color c, float inten) : Light(p, c, inten) {}
  virtual ~PointLight() {}

  // A point light isn't an area, so we can only take one sample.
  virtual int getNumSamples() const { return 1; }

  virtual void sample(Vector &point) const { point = position; }
};

#endif
