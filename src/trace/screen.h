#ifndef __SCREEN_H_
#define __SCREEN_H_

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <math.h>
#include "../core/color.h"
#include "../core/vector3.h"

class Screen {
public:
  Screen(int _w, int _h);
  ~Screen();

  // Writes out the buffer to disk, as a TGA file.
  bool writeImage(const std::string& file);

  int getWidth() const {
    return width;
  }
  int getHeight() const {
    return height;
  }

  void setPixel(int x, int y, const Color &color) {
    if (x < 0 || y < 0 || x >= width || y >= height) {
      printf("pixel (%d, %d) out of range...\n", x, y);
      return;
    }
    data[x + y * width] = color;

    max_component = std::max(max_component, color.r);
    max_component = std::max(max_component, color.g);
    max_component = std::max(max_component, color.b);
  }

private:
  int width;
  int height;
  Color *data;
  float max_component;
};

class Camera {
public:
  Camera(const Vector& p, const Vector& u, const Vector& l)
    : pos(p), up(u), lookAt(l) {
    init();
  }
  Camera() {
    pos = Vector(0, 0, 10);
    up = Vector(0, 1, 0);
    lookAt = Vector(0, 0, 0);
    init();
  }
  ~Camera() = default;

  Vector u, v, w;
  Vector pos, up, lookAt;
  int camWidth;

private:
  void init() {
    w = (lookAt - pos).normalize();
    u = up.cross(w).normalize();
    v = w.cross(u);
    camWidth = 1000;
  }
};

#endif
