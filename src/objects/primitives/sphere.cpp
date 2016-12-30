#include "sphere.h"

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <math.h>

#include "../../core/util.h"

bool Sphere::contains(const Vector& point) const {
  const float r = radius + 1e-6;
  return (point - getCenter()).length2() <= r * r;
}

bool Sphere::intersect(const Ray& ray, Intersection& intersection) {
  const Vector origin = ray.origin;
  Vector direction = ray.direction;
  direction.normalize();

  // Calculate the coefficients of the quadratic equation
  // au^2 + bu + c = 0. We seek the value of u for each intersection.
  const Vector displace = origin - getCenter();
  const float a = direction.dot(direction);
  const float b = 2.0 * displace.dot(direction);
  const float c = displace.dot(displace) - radius * radius;
  float radicand = b * b - 4 * a * c;

  if (radicand < 0.0)
    return false;

  radicand = sqrtf(radicand);
  const float r1 = (-b - radicand) * 0.5;
  const float r2 = (-b + radicand) * 0.5;

  float dist = r1;
  if (dist < 0)
    dist = r2;
  if (dist < 0)
    return false;

  const Vector pt = origin + (direction * dist);
  Vector nml = (pt - getCenter()).normalize();

  const float intersect_dist = (pt - ray.origin).length2();

  // The intersection is too far from the origin of the ray.
  if (intersect_dist >= RAY_FAR_DISTANCE)
    return false;

  if (direction.dot(nml) > 0)
    nml *= -1;

  intersection.object = this;
  intersection.pt = pt;
  intersection.nml = nml;
  intersection.ray = ray;
  for (const auto& pair : textureMap) {
    intersection.textureName = pair.first;
  }
  intersection.finalized = true;
  return true;
}

int SphereLight::getNumSamples() const { return 8 * 8; }

void SphereLight::sample(Vector& pos) const {

  Vector random_unit_vector;

  const float angle = UTILrandomFloatBetween(0.0, 2.0 * M_PI);
  const float x = UTILrandomFloatBetween(0.0, 2.0) - 1.0;
  const float s = sqrtf(1.0 - x * x);

  random_unit_vector.x = x;
  random_unit_vector.y = s * cos(angle);
  random_unit_vector.z = s * sin(angle);

  // A random point on the surface of the sphere.
  pos = position + random_unit_vector * radius;
}
