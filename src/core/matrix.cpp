#include "matrix.h"

#include <math.h>

Matrix::Matrix() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      data[i][j] = i == j ? 1 : 0;
    }
  }
}

Matrix::Matrix(const Matrix& other) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      data[i][j] = other[i][j];
    }
  }
}

void Matrix::set(const int i, const int j, const float val) {
  data[i][j] = val;
}

void Matrix::setRotation(const Vector& axis, const float degrees) {
  float ct = static_cast<float>(cos(M_PI * degrees / 180.0));
  float st = static_cast<float>(sin(M_PI * degrees / 180.0));
  if (ct == 1) {
    return;
  }

  float t = 1 - ct;
  float tx = t * axis.x;
  float ty = t * axis.y;
  float tz = t * axis.z;
  float txy = tx * axis.y;
  float txz = tx * axis.z;
  float tyz = ty * axis.z;
  float sx = st * axis.x;
  float sy = st * axis.y;
  float sz = st * axis.z;

  data[0][0] = tx * axis.x + ct;
  data[0][1] = txy + sz;
  data[0][2] = txz - sy;

  data[1][0] = txy - sz;
  data[1][1] = ty * axis.y + ct;
  data[1][2] = tyz + sx;

  data[2][0] = txz + sy;
  data[2][1] = tyz - sx;
  data[2][2] = tz * axis.z + ct;
}

std::ostream& operator<<(std::ostream& os, const Matrix& mat) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      os << mat[i][j] << " ";
    }
    if (i != 2) {
      os << std::endl;
    }
  }
  return os;
}

Matrix Matrix::operator+(const Matrix& other) const {
  Matrix ret;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ret.set(i, j, data[i][j] + other[i][j]);
    }
  }
  return ret;
};

Matrix Matrix::operator-(const Matrix& other) const {
  Matrix ret;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ret.set(i, j, data[i][j] - other[i][j]);
    }
  }
  return ret;
};

const float* Matrix::operator[](int i) const {
  return data[i];
}

Vector operator*(const Matrix& mat, const Vector& pt) {
  return Vector(
    pt.x * mat[0][0] + pt.y * mat[0][1] + pt.z * mat[0][2],
    pt.x * mat[1][0] + pt.y * mat[1][1] + pt.z * mat[1][2],
    pt.x * mat[2][0] + pt.y * mat[2][1] + pt.z * mat[2][2]);
}
