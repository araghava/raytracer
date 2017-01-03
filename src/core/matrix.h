#ifndef __MATRIX_H_
#define __MATRIX_H_

#include "geometry.h"

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
  Matrix operator*(const Matrix& other) const;
  const float* operator[](int i) const;
  friend std::ostream& operator<<(std::ostream& o, const Matrix& mat);

  static Matrix fromRotation(const Vector& axis, const float degrees);

 private:
  float data[3][3];
};

Vector operator*(const Matrix& mat, const Vector& pt);

struct Transform {
  Matrix rotation;
  Vector translation;
  Vector scale = Vector(1, 1, 1);
};

// 4x4 matrix class (mostly taken from pbrt-v3)
struct Matrix4 {
  Matrix4();
  Matrix4(const Matrix4& o);
  Matrix4(float mat[4][4]);
  Matrix4(float m00, float m01, float m02, float m03,
          float m10, float m11, float m12, float m13,
          float m20, float m21, float m22, float m23,
          float m30, float m31, float m32, float m33);

  static Matrix4 multiply(const Matrix4& m1, const Matrix4& m2);
  friend Matrix4 MatrixInvert(const Matrix4& mat);
  friend Matrix4 MatrixTranspose(const Matrix4& mat);
  friend std::ostream& operator<<(std::ostream& os, const Matrix4& m);

  bool operator==(const Matrix4& o) const;
  bool operator!=(const Matrix4& o) const;

  float m[4][4];
};

class Transform2 {
 public:
  Transform2() = default;
  Transform2(const float mat[4][4]);
  Transform2(const Matrix4& mat);
  Transform2(const Matrix4& mat, const Matrix4& matInv);

  friend Transform2 Transform2Inverse(const Transform2& t);
  friend Transform2 Transform2Transpose(const Transform2& t);
  friend std::ostream& operator<<(std::ostream& os, const Matrix4& m);

  bool operator==(const Transform2& t) const;
  bool operator!=(const Transform2& t) const;

  bool isIdentity() const;
  Matrix4 getMatrix() const;
  Matrix4 getInverseMatrix() const;

  inline Vector operator()(const Vector& v) const;
 private:
  Matrix4 m, mInv;
};


#endif
