#ifndef __TEXTURE_H
#define __TEXTURE_H

#include <string>

#include "../core/vector3.h"
#include "../core/color.h"

// Texture applicable to objects
class Texture {
public:
  Texture(const std::string& s) : id(s) {}
  virtual ~Texture() = default;

  virtual Color sample(const double i, const double j) const = 0;

  std::string getId() const {
    return id;
  }

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

private:
  Color color;
};

class NoopTexture : public SolidTexture {
public:
  NoopTexture(const std::string id) : SolidTexture(id, Color(1, 1, 1)) {}
};

#endif
