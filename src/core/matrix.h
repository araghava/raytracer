#ifndef __MATRIX_H_
#define __MATRIX_H_

#include "vector3.h"

#include <ostream>

class Matrix {
 public:
  Matrix();
  Matrix(const Matrix& other);
  ~Matrix() = default;

  void print();
  void set(const int i, const int j, const float val);
  void setRotation(const Vector& axis, const float degrees);
  Vector applyTransform(const Vector& pt);

  Matrix operator+(const Matrix& other) const;
  Matrix operator-(const Matrix& other) const;
  const float* operator[](int i) const;
  friend std::ostream& operator<<(std::ostream& o, const Matrix& mat);

 private:
  float data[3][3];
};

Vector operator*(const Matrix& mat, const Vector& pt);

struct Transform {
  Matrix rotation;
  Vector translation;
};

#endif
