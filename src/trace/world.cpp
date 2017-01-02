#include "world.h"

#include <cstdlib>
#include <cfloat>
#include <iostream>
#include <math.h>

#include "../core/constants.h"

bool World::getClosestIntersection(const Ray& ray, Intersection& intersect) {
  float min_dist = std::numeric_limits<float>::max();
  bool found = false;

  // check each object for intersection
  for (int i = 0; i < (int)objectList.size(); i++) {
    Intersection inter;
    if (objectList[i]->intersect(ray, inter)) {
      found = true;
      float dist = (inter.pt - ray.origin).length2();

      // if this is closer, record this one
      if (dist < min_dist) {
        min_dist = dist;
        intersect = inter;
      }
    }
  }

  return found;
}

Color World::traceRay(const Ray& ray) {
  Intersection inter;
  if (getClosestIntersection(ray, inter) && inter.finalized) {
    return computeLighting(inter);
  }

  return Color(0, 0, 0);
}

bool World::castShadowRay(const Vector& position,
                          const Intersection& intersect) {
  Ray shadow_ray(intersect.pt, (position - intersect.pt).normalize(), -1);
  Intersection inter;
  float lightdist2 = (position - intersect.pt).length2();

  if (getClosestIntersection(shadow_ray, inter)) {
    float dist2 = (inter.pt - intersect.pt).length2();
    if (dist2 < lightdist2)
      return true;
  }

  return false;
}

Color World::computeRefractiveReflective(const Intersection& intersect) {
  if (intersect.ray.remaining_casts <= 0) {
    return Color(0, 0, 0);
  }

  Vector incident = intersect.pt - intersect.ray.origin;
  Ray r(intersect.pt,
        UTILreflectVector(incident, intersect.nml),
        intersect.ray.remaining_casts - 1);

  return traceRay(r) * RAY_CAST_ATTENUATION;
}

Color World::computeLighting(const Intersection& intersect) {
  Color ret;

  for (int i = 0; i < (int)lightList.size(); i++) {
    // sample the light source for soft shadows (with area lights)
    int samples = lightList[i]->getNumSamples();
    Color curLight;

    for (int j = 0; j < samples; j++) {
      Vector sample_pos = lightList[i]->sample();
      if (!castShadowRay(sample_pos, intersect)) {
        curLight += objectList[intersect.objectId]->getTexture()->shade(
          intersect, lightList[i], sample_pos);
      }
    }

    curLight /= (1.0 * samples);
    ret += curLight;
  }

  return ret + computeRefractiveReflective(intersect);
}
