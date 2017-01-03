#ifndef __TRANSFORM_H_
#define __TRANSFORM_H_

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

class Transform {
 public:
  Transform() = default;
  Transform(const float mat[4][4]);
  Transform(const Matrix4& mat);
  Transform(const Matrix4& mat, const Matrix4& matInv);

  friend Transform TransformInverse(const Transform& t);
  friend Transform TransformTranspose(const Transform& t);
  friend std::ostream& operator<<(std::ostream& os, const Matrix4& m);

  bool operator==(const Transform& t) const;
  bool operator!=(const Transform& t) const;

  bool isIdentity() const;
  Matrix4 getMatrix() const;
  Matrix4 getInverseMatrix() const;

  Transform operator*(const Transform& o) const;
  Vector applyPoint(const Vector& p) const;
  Vector applyVector(const Vector& v) const;

  static Transform translate(const Vector& delta);
  static Transform scale(float x, float y, float z);
  static Transform rotateX(float theta);
  static Transform rotateY(float theta);
  static Transform rotateZ(float theta);
  static Transform rotate(float theta, const Vector& axis);
  static Transform lookAt(const Vector& pos, const Vector& look, const Vector& up);
  static Transform perspective(float fov, float znear, float zfar);

 private:
  Matrix4 m, mInv;
};

#endif
