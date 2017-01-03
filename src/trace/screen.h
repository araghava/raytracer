#ifndef __SCREEN_H_
#define __SCREEN_H_

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <math.h>
#include "../core/color.h"
#include "../core/geometry.h"

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
    : pos(p), up(u), dir(l) {
    init();
  }
  Camera() {
    pos = Vector(0, 0, 0);
    up = Vector(0, 1, 0);
    dir = Vector(0, 0, -1);
    width = 1024;
    height = 1024;
    antialias = 4;
    init();
  }
  ~Camera() = default;

  Vector u, v, w;
  Vector pos, up, dir;
  int width, height, antialias;

private:
  void init() {
    w = (dir - pos).normalize();
    u = up.cross(w).normalize();
    v = w.cross(u);
  }
};

#endif
