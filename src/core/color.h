#ifndef __COLOR_H_
#define __COLOR_H_

// Utility color class.
class Color {
public:
  float r, g, b, a;
  Color() : r(0.), g(0.), b(0.), a(1.) {}
  Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1.) {}
  Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}

  // Common operator overloads.
  Color operator+(const Color &c) const {
    return Color(r + c.r, g + c.g, b + c.b);
  }
  Color &operator+=(const Color &c) {
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
  }

  Color operator-(const Color &c) const {
    return Color(r - c.r, g - c.g, b - c.b);
  }
  Color &operator-=(const Color &c) {
    r -= c.r;
    g -= c.g;
    b -= c.b;
    return *this;
  }

  Color operator*(float scale) const {
    return Color(scale * r, scale * g, scale * b);
  }
  Color &operator*=(float scale) {
    r *= scale;
    g *= scale;
    b *= scale;
    return *this;
  }

  Color operator/(float scale) const {
    return Color(r / scale, g / scale, b / scale);
  }
  Color &operator/=(float scale) {
    r /= scale;
    g /= scale;
    b /= scale;
    return *this;
  }
};

#endif
