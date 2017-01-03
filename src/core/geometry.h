#ifndef __VECTOR_H_
#define __VECTOR_H_

#include <ostream>

// Utility 3-D vector class.
class Vector {
public:
  float x, y, z;

  Vector() : x(0.), y(0.), z(0.) {}
  Vector(float f) : x(f), y(f), z(f) {}
  Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

  Vector& normalize();

  Vector cross(const Vector& v) const;
  float dot(const Vector& v) const;

  // Squared length, useful for optimized distance comparisons.
  float length2() const;
  float length() const;

  // Common operator overrides.
  Vector operator+(const Vector& v) const;
  Vector &operator+=(const Vector& v);

  Vector operator-(const Vector& v) const;
  Vector &operator-=(const Vector& v);

  Vector operator*(const Vector& v) const;
  Vector &operator*=(const Vector& v);

  Vector operator/(const Vector& v) const;
  Vector &operator/=(const Vector& v);

  Vector operator*(float scale) const;
  Vector &operator*=(float scale);

  Vector operator/(float scale) const;
  Vector &operator/=(float scale);

  float operator[](size_t idx) const;

  friend std::ostream& operator<<(std::ostream& o, const Vector &a);
};

// Utility 3-D point class. We differentiate this from vector as translation
// of a vector is meaningless in our case.
class Point {
public:
  float x, y, z;

  Point() : x(0.), y(0.), z(0.) {}
  Point(float f) : x(f), y(f), z(f) {}
  Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

  // Common operator overrides.
  Point operator+(const Point& v) const;
  Point &operator+=(const Point& v);

  Point operator-(const Point& v) const;
  Point &operator-=(const Point& v);

  Point operator*(const Point& v) const;
  Point &operator*=(const Point& v);

  Point operator/(const Point& v) const;
  Point &operator/=(const Point& v);

  Point operator*(float scale) const;
  Point &operator*=(float scale);

  Point operator/(float scale) const;
  Point &operator/=(float scale);

  float operator[](size_t idx) const;

  friend std::ostream& operator<<(std::ostream& o, const Point &a);
};

#endif
