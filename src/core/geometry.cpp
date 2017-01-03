#include "geometry.h"

#include <math.h>
#include <stdexcept>

Vector &Vector::normalize() { return (*this) /= this->length(); }

Vector Vector::cross(const Vector& v) const {
  // Take the determinant of the 3x3 matrix with i, j, k at the top row
  // and the other two vectors under.
  return Vector(y * v.z - v.y * z, v.x * z - x * v.z, x * v.y - v.x * y);
}

float Vector::dot(const Vector& v) const { return x * v.x + y * v.y + z * v.z; }

float Vector::length2() const { return x * x + y * y + z * z; }

float Vector::length() const { return sqrtf(length2()); }

Vector Vector::operator+(const Vector& v) const {
  return Vector(x + v.x, y + v.y, z + v.z);
}
Vector &Vector::operator+=(const Vector& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vector Vector::operator-(const Vector& v) const {
  return Vector(x - v.x, y - v.y, z - v.z);
}
Vector &Vector::operator-=(const Vector& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector Vector::operator*(const Vector& v) const {
  return Vector(x * v.x, y * v.y, z * v.z);
}
Vector &Vector::operator*=(const Vector& v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
  return *this;
}

Vector Vector::operator/(const Vector& v) const {
  return Vector(x / v.x, y / v.y, z / v.z);
}
Vector &Vector::operator/=(const Vector& v) {
  x /= v.x;
  y /= v.y;
  z /= v.z;
  return *this;
}

Vector Vector::operator*(float scale) const {
  return Vector(scale * x, scale * y, scale * z);
}
Vector &Vector::operator*=(float scale) {
  x *= scale;
  y *= scale;
  z *= scale;
  return *this;
}

Vector Vector::operator/(float scale) const {
  return Vector(scale / x, scale / y, scale / z);
}
Vector &Vector::operator/=(float scale) {
  x /= scale;
  y /= scale;
  z /= scale;
  return *this;
}

float Vector::operator[](size_t idx) const {
  if (idx == 0) return x;
  else if (idx == 1) return y;
  else if (idx == 2) return z;
  throw std::out_of_range ("idx should be within [0, 2]");
}

std::ostream& operator<<(std::ostream& os, const Vector& v) {
  os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}

Point Point::operator+(const Point& v) const {
  return Point(x + v.x, y + v.y, z + v.z);
}
Point &Point::operator+=(const Point& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Point Point::operator-(const Point& v) const {
  return Point(x - v.x, y - v.y, z - v.z);
}
Point &Point::operator-=(const Point& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Point Point::operator*(const Point& v) const {
  return Point(x * v.x, y * v.y, z * v.z);
}
Point &Point::operator*=(const Point& v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
  return *this;
}

Point Point::operator/(const Point& v) const {
  return Point(x / v.x, y / v.y, z / v.z);
}
Point &Point::operator/=(const Point& v) {
  x /= v.x;
  y /= v.y;
  z /= v.z;
  return *this;
}

Point Point::operator*(float scale) const {
  return Point(scale * x, scale * y, scale * z);
}
Point &Point::operator*=(float scale) {
  x *= scale;
  y *= scale;
  z *= scale;
  return *this;
}

Point Point::operator/(float scale) const {
  return Point(scale / x, scale / y, scale / z);
}
Point &Point::operator/=(float scale) {
  x /= scale;
  y /= scale;
  z /= scale;
  return *this;
}

float Point::operator[](size_t idx) const {
  if (idx == 0) return x;
  else if (idx == 1) return y;
  else if (idx == 2) return z;
  throw std::out_of_range ("idx should be within [0, 2]");
}

std::ostream& operator<<(std::ostream& os, const Point& v) {
  os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}
