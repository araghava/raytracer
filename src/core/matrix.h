#ifndef __MATRIX_H_
#define __MATRIX_H_

#include <iostream>
#include <ostream>

class Matrix {
 public:
  Matrix();
  Matrix(const Matrix& other);
  ~Matrix() = default;

  void print();
  void set(const int i, const int j, const float val);

  Matrix operator+(const Matrix& other) const;
  Matrix operator-(const Matrix& other) const;
  const float* operator[](int i) const;

  friend std::ostream& operator<<(std::ostream& o, const Matrix& mat);

 private:
  float data[3][3];
};

#endif
