#include "transform.h"

#include <math.h>
#include <string.h>

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

Matrix Matrix::fromRotation(const Vector& axis, const float degrees) {
  Matrix rot;
  rot.setRotation(axis, degrees);
  return rot;
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

Matrix Matrix::operator*(const Matrix& a) const {
  Matrix ret;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ret.set(i, j, 0);
    }
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
        ret.set(i, j, data[i][k] * a[k][j]);
      }
    }
  }
  return ret;
}

Vector operator*(const Matrix& mat, const Vector& pt) {
  return Vector(
    pt.x * mat[0][0] + pt.y * mat[0][1] + pt.z * mat[0][2],
    pt.x * mat[1][0] + pt.y * mat[1][1] + pt.z * mat[1][2],
    pt.x * mat[2][0] + pt.y * mat[2][1] + pt.z * mat[2][2]);
}

Matrix4::Matrix4() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      m[i][j] = i == j ? 1 : 0;
    }
  }
}

Matrix4::Matrix4(const Matrix4& o) {
  memcpy(m, o.m, 4 * 4 * sizeof(float));
}

Matrix4::Matrix4(float mat[4][4]) {
  memcpy(m, mat, 4 * 4 * sizeof(float));
}

Matrix4::Matrix4(float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33) {
  m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
  m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
  m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
  m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}

bool Matrix4::operator==(const Matrix4& o) const {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (m[i][j] != o.m[i][j]) {
        return false;
      }
    }
  }
  return true;
}

bool Matrix4::operator!=(const Matrix4& o) const {
  return !((*this) == o);
}

Matrix4 MatrixTranspose(const Matrix4& mat) {
  return Matrix4(
    mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
    mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
    mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
    mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]);
}

Matrix4 MatrixInvert(const Matrix4& mat) {
  int indxc[4], indxr[4];
  int ipiv[4] = {0, 0, 0, 0};
  float minv[4][4];
  memcpy(minv, mat.m, 4 * 4 * sizeof(float));

  for (int i = 0; i < 4; i++) {
    int irow = 0, icol = 0;
    float big = 0.f;

    // choose pivot
    for (int j = 0; j < 4; j++) {
      if (ipiv[j] != 1) {
        for (int k = 0; k < 4; k++) {
          if (ipiv[k] == 0) {
            if (fabs(minv[j][k]) >= big) {
              big = float(fabs(minv[j][k]));
              irow = j;
              icol = k;
            }
          } else if (ipiv[k] > 1) {
            printf("Singular matrix in invert");
            return Matrix4();
          }
        }
      }
    }

    ++ipiv[icol];

    // swap rows irow and icol for pivot
    if (irow != icol) {
      for (int k = 0; k < 4; ++k) {
        std::swap(minv[irow][k], minv[icol][k]);
      }
    }
    indxr[i] = irow;
    indxc[i] = icol;
    if (minv[icol][icol] == 0.f) {
      printf("Singular matrix in invert");
      return Matrix4();
    }

    // Set m[icol][icol] to one by scaling row icol appropriately
    float pivinv = 1. / minv[icol][icol];
    minv[icol][icol] = 1.;
    for (int j = 0; j < 4; j++) {
      minv[icol][j] *= pivinv;
    }

    // subtract this row from others to zero out their columns
    for (int j = 0; j < 4; j++) {
      if (j != icol) {
        float save = minv[j][icol];
        minv[j][icol] = 0;
        for (int k = 0; k < 4; k++) {
          minv[j][k] -= minv[icol][k] * save;
        }
      }
    }
  }

  // swap columns to reflect permutation
  for (int j = 3; j >= 0; j--) {
    if (indxr[j] != indxc[j]) {
      for (int k = 0; k < 4; k++) {
        std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
      }
    }
  }

  return Matrix4(minv);
}

std::ostream& operator<<(std::ostream& os, const Matrix4& m) {
  os << "[" << std::endl;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      os << " " << m.m[i][j];
    }
    os << std::endl;
  }
  os << "]" << std::endl;
  return os;
}

Matrix4 Matrix4::multiply(const Matrix4& m1, const Matrix4& m2) {
  Matrix4 ret;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ret.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] +
                    m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
    }
  }
  return ret;
}

Transform2::Transform2(const float mat[4][4]) {
  m = Matrix4(
    mat[0][0], mat[0][1], mat[0][2], mat[0][3],
    mat[1][0], mat[1][1], mat[1][2], mat[1][3],
    mat[2][0], mat[2][1], mat[2][2], mat[2][3],
    mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
  mInv = MatrixInvert(m);
}

Transform2::Transform2(const Matrix4& mat) {
  m = mat;
  mInv = MatrixInvert(mat);
}

Transform2::Transform2(const Matrix4& mat, const Matrix4& inv) {
  m = mat;
  mInv = inv;
}

Transform2 Transform2Inverse(const Transform2& t) {
  return Transform2(t.mInv, t.m);
}

Transform2 Transform2Transpose(const Transform2& t) {
  return Transform2(MatrixTranspose(t.m), MatrixTranspose(t.mInv));
}

bool Transform2::operator==(const Transform2& t) const {
  return t.m == m && t.mInv == mInv;
}

bool Transform2::operator!=(const Transform2& t) const {
  return t.m != m || t.mInv != t.mInv;
}

Matrix4 Transform2::getMatrix() const {
  return m;
}

Matrix4 Transform2::getInverseMatrix() const {
  return mInv;
}

// apply transformation as a point (translation + rotation)
inline Vector Transform2::applyPoint(const Vector& p) const {
  float x = p.x, y = p.y, z = p.z;
  float xp = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
  float yp = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
  float zp = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
  float wp = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];
  return wp == 1 ? Vector(xp, yp, zp) : Vector(xp, yp, zp) / wp;
}

// apply transformation as vector (rotation only)
inline Vector Transform2::applyVector(const Vector& v) const {
  float x = v.x, y = v.y, z = v.z;
  return Vector(
    m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z,
    m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z,
    m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z);
}

Transform2 Transform2::translate(const Vector& delta) {
  Matrix4 m(
    1, 0, 0, delta.x,
    0, 1, 0, delta.y,
    0, 0, 1, delta.z,
    0, 0, 0, 1);
  Matrix4 minv(1, 0, 0, -delta.x,
               0, 1, 0, -delta.y,
               0, 0, 1, -delta.z,
               0, 0, 0, 1);
  return Transform2(m, minv);
}

Transform2 Transform2::scale(float x, float y, float z) {
  Matrix4 m(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
  Matrix4 minv(1 / x, 0, 0, 0, 0, 1 / y, 0, 0, 0, 0, 1 / z, 0, 0, 0, 0, 1);
  return Transform2(m, minv);
}

Transform2 Transform2::rotateX(float theta) {
  float sinTheta = sin(theta * M_PI / 180.0);
  float cosTheta = cos(theta * M_PI / 180.0);
  Matrix4 m(
    1, 0, 0, 0,
    0, cosTheta, -sinTheta, 0,
    0, sinTheta, cosTheta, 0,
    0, 0, 0, 1);
  return Transform2(m, MatrixTranspose(m));
}

Transform2 Transform2::rotateY(float theta) {
  float sinTheta = sin(theta * M_PI / 180.0);
  float cosTheta = cos(theta * M_PI / 180.0);
  Matrix4 m(cosTheta, 0, sinTheta, 0,
            0, 1, 0, 0,
            -sinTheta, 0, cosTheta, 0,
            0, 0, 0, 1);
  return Transform2(m, MatrixTranspose(m));
}

Transform2 Transform2::rotateZ(float theta) {
  float sinTheta = sin(theta * M_PI / 180.0);
  float cosTheta = cos(theta * M_PI / 180.0);
  Matrix4 m(cosTheta, -sinTheta, 0, 0,
            sinTheta, cosTheta, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
  return Transform2(m, MatrixTranspose(m));
}

Transform2 Transform2::rotate(float theta, const Vector& a) {
  float sinTheta = sin(theta * M_PI / 180.0);
  float cosTheta = cos(theta * M_PI / 180.0);
  Matrix4 m;

  // compute rotation of first basis vector
  m.m[0][0] = a.x * a.x + (1 - a.x * a.x) * cosTheta;
  m.m[0][1] = a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
  m.m[0][2] = a.x * a.z * (1 - cosTheta) + a.y * sinTheta;
  m.m[0][3] = 0;

  // compute rotations of second and third basis vectors
  m.m[1][0] = a.x * a.y * (1 - cosTheta) + a.z * sinTheta;
  m.m[1][1] = a.y * a.y + (1 - a.y * a.y) * cosTheta;
  m.m[1][2] = a.y * a.z * (1 - cosTheta) - a.x * sinTheta;
  m.m[1][3] = 0;

  m.m[2][0] = a.x * a.z * (1 - cosTheta) - a.y * sinTheta;
  m.m[2][1] = a.y * a.z * (1 - cosTheta) + a.x * sinTheta;
  m.m[2][2] = a.z * a.z + (1 - a.z * a.z) * cosTheta;
  m.m[2][3] = 0;
  return Transform2(m, MatrixTranspose(m));
}

Transform2 Transform2::lookAt(const Vector& pos, const Vector& look, const Vector& up) {
  Matrix4 cameraToWorld;

  // initialize fourth column of viewing matrix
  cameraToWorld.m[0][3] = pos.x;
  cameraToWorld.m[1][3] = pos.y;
  cameraToWorld.m[2][3] = pos.z;
  cameraToWorld.m[3][3] = 1;

  // initialize first three columns of viewing matrix
  Vector dir = (look - pos).normalize();
  if ((up.cross(dir)).length() == 0) {
    return Transform2();
  }

  Vector left = (up.cross(dir)).normalize();
  Vector newUp = dir.cross(left);
  cameraToWorld.m[0][0] = left.x;
  cameraToWorld.m[1][0] = left.y;
  cameraToWorld.m[2][0] = left.z;
  cameraToWorld.m[3][0] = 0.;
  cameraToWorld.m[0][1] = newUp.x;
  cameraToWorld.m[1][1] = newUp.y;
  cameraToWorld.m[2][1] = newUp.z;
  cameraToWorld.m[3][1] = 0.;
  cameraToWorld.m[0][2] = dir.x;
  cameraToWorld.m[1][2] = dir.y;
  cameraToWorld.m[2][2] = dir.z;
  cameraToWorld.m[3][2] = 0.;
  return Transform2(MatrixInvert(cameraToWorld), cameraToWorld);
}

Transform2 Transform2::perspective(float fov, float n, float f) {
  // perform projective divide for perspective projection
  Matrix4 persp(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, f / (f - n), -f * n / (f - n),
                0, 0, 1, 0);

  // scale canonical perspective view to specified field of view
  float invTanAng = 1 / tan(fov * M_PI / (180.0 * 2));
  return scale(invTanAng, invTanAng, 1) * Transform2(persp);
}

Transform2 Transform2::operator*(const Transform2& o) const {
  return Transform2(Matrix4::multiply(m, o.m), Matrix4::multiply(o.mInv, mInv));
}
