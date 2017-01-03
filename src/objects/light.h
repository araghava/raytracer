#ifndef __LIGHT_H
#define __LIGHT_H

#include "../core/color.h"
#include "../core/matrix.h"
#include "../core/geometry.h"

// Abstract Light.
class Light {
public:
  Light(Color c, float inten) : color(c), intensity(inten) {
  }
  virtual ~Light() = default;

  // This gets the number of samples to use when trying to determine luminance
  // from a light. This is needed for area lights (to create soft shadows).
  virtual int getNumSamples() const = 0;

  // Uniform random sampling on the area of this light. Fills "point" with a
  // random
  // point on the surface of the light object.
  virtual Vector sample() const = 0;

  void setTransform(const Transform& t) {
    position = t.translation;
  }

  Vector position;
  Color color;
  float intensity;
};

class PointLight : public Light {
public:
  PointLight(Color c, float inten) : Light(c, inten) {
  }
  virtual ~PointLight() = default;

  // A point light isn't an area, so we can only take one sample.
  virtual int getNumSamples() const {
    return 1;
  }

  virtual Vector sample() const {
    return position;
  }
};

#endif
