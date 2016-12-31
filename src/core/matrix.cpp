#include "matrix.h"

Matrix::Matrix() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      data[i][j] = 0;
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
