#include "util.h"
#include <cstdlib>
#include "vector3.h"

float UTILrandomFloatBetween(float min, float max) {
  const float r = ((float)rand()) / (float)RAND_MAX;
  return (r * (max - min)) + min;
}

Vector UTILreflectVector(const Vector& incident, const Vector& nml) {
  const float perp = 2.0 * incident.dot(nml);
  return (incident - (nml * perp)).normalize();
}

Vector UTILinterpolateFace(const std::vector<Vector>& values, const Vector& bc) {
  return (values[0] * bc.x) + (values[1] * bc.y) + (values[2] * bc.z);
}
