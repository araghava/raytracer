#include "screen.h"
#include <fstream>
#include <iostream>

Screen::Screen(int _w, int _h) {
  width = _w;
  height = _h;
  max_component = -1;
  data = (Color *)malloc(sizeof(Color) * width * height);
}

Screen::~Screen() {
  free(data);
  data = 0;
}

bool Screen::writeImage(const std::string& file) {
  std::ofstream o(file.c_str(), std::ios::out | std::ios::binary);

  // Write the header.
  o.put(0);
  o.put(0);

  // Uncompressed RGB format.
  o.put(2);
  o.put(0);
  o.put(0);
  o.put(0);
  o.put(0);
  o.put(0);
  o.put(0);
  o.put(0);
  o.put(0);
  o.put(0);
  o.put((width & 0x00FF));
  o.put((width & 0xFF00) / 256);
  o.put((height & 0x00FF));
  o.put((height & 0xFF00) / 256);
  o.put(32);
  o.put(0);

  for (int i = 0; i < width * height; i++) {
    // Scale everything to maximum color, in order for color values to
    // be in the range [0, 255].
    o.put((unsigned char)((data[i].b / max_component) * 255));
    o.put((unsigned char)((data[i].g / max_component) * 255));
    o.put((unsigned char)((data[i].r / max_component) * 255));
    o.put((unsigned char)(data[i].a * 255));
  }

  o.close();
  return true;
}
