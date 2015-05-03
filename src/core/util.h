#ifndef __UTIL_H_
#define __UTIL_H_

#include <cstdlib>
#include "vector3.h"

// Returns a random float in the range [min, max].
extern float UTILrandomFloatBetween(float min, float max);

// Reflects an incident vector across a given surface normal.
// Returns a normalized vector.
extern Vector UTILreflectVector (const Vector &incident, const Vector &nml);

#endif
