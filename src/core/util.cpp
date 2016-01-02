#ifndef __UTIL_H_
#define __UTIL_H_

#include <cstdlib>
#include "util.h"
#include "vector3.h"

float UTILrandomFloatBetween(float min, float max) {
  float r = ((float)rand()) / (float)RAND_MAX;
  return (r * (max - min)) + min;
}

Vector UTILreflectVector(const Vector &incident, const Vector &nml) {
  float perp = 2.0 * incident.dot(nml);
  return (incident - (nml * perp)).normalize();
}

#endif
