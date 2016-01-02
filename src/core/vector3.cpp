#include "vector3.h"
#include <math.h>

Vector &Vector::normalize() { return (*this) /= this->length(); }

Vector Vector::cross(const Vector &v) const {
  // Take the determinant of the 3x3 matrix with i, j, k at the top row
  // and the other two vectors under.
  return Vector(y * v.z - v.y * z, v.x * z - x * v.z, x * v.y - v.x * y);
}

float Vector::dot(const Vector &v) const { return x * v.x + y * v.y + z * v.z; }

float Vector::length2() const { return x * x + y * y + z * z; }

float Vector::length() const { return sqrtf(length2()); }

Vector Vector::operator+(const Vector &v) const {
  return Vector(x + v.x, y + v.y, z + v.z);
}
Vector &Vector::operator+=(const Vector &v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vector Vector::operator-(const Vector &v) const {
  return Vector(x - v.x, y - v.y, z - v.z);
}
Vector &Vector::operator-=(const Vector &v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector Vector::operator*(const Vector &v) const {
  return Vector(x * v.x, y * v.y, z * v.z);
}
Vector &Vector::operator*=(const Vector &v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
  return *this;
}

Vector Vector::operator/(const Vector &v) const {
  return Vector(x / v.x, y / v.y, z / v.z);
}
Vector &Vector::operator/=(const Vector &v) {
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
