#ifndef __UTIL_H_
#define __UTIL_H_

#include <cstdlib>
#include <vector>

#include "../external/tiny_obj_loader.h"
#include "geometry.h"
#include "../objects/object.h"

// Returns a random float in the range [min, max].
extern float UTILrandomFloatBetween(const float min, const float max);

// Reflects an incident vector across a given surface normal.
// Returns a normalized vector.
extern Vector UTILreflectVector(const Vector& incident, const Vector& nml);

// Interpolates values along the triangluar face and returns the value at
// the barycentric coordinate bc.
extern Vector UTILinterpolateFace(const std::vector<Vector>& values, const Vector& bc);

extern Vector UTILgetFaceVertex(
  const std::array<tinyobj::index_t, 3>& face, const tinyobj::attrib_t& attrib, size_t v);

extern Vector UTILgetFaceNormal(
  const std::array<tinyobj::index_t, 3>& face, const tinyobj::attrib_t& attrib, size_t v);

extern Vector UTILgetFaceTexCoord(
  const std::array<tinyobj::index_t, 3>& face, const tinyobj::attrib_t& attrib, size_t v);

extern Vector UTILtransformVector(
  const Vector& vector, const Transform& transform);

#endif
