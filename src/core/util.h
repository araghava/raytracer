#ifndef __UTIL_H_
#define __UTIL_H_

#include <cstdlib>
#include <vector>

#include "vector3.h"

// Returns a random float in the range [min, max].
extern float UTILrandomFloatBetween(const float min, const float max);

// Reflects an incident vector across a given surface normal.
// Returns a normalized vector.
extern Vector UTILreflectVector(const Vector& incident, const Vector& nml);

// Interpolates values along the triangluar face and returns the value at
// the barycentric coordinate bc.
extern Vector UTILinterpolateFace(const std::vector<Vector>& values, const Vector& bc);

#endif
