#include "world.h"

#include <cstdlib>
#include <cfloat>
#include <iostream>
#include <math.h>

#include "../core/constants.h"

Color World::traceRay(const Ray& ray) {
  Intersection inter;
  if (getClosestIntersection(ray, inter)) {
    return computeLighting(inter);
  }
  return Color(0, 0, 0);
}

Color World::computeDiffuse(const std::shared_ptr<Light>& light,
                            const Intersection& intersect,
                            float spec_contrib, float& diff_contrib,
                            const Vector& sample_pos) {
  Vector pos_off = (sample_pos - intersect.pt).normalize();

  // The amount this light contributes is proportional to the dot
  // product of the relative light position and surface normal.
  diff_contrib = intersect.nml.dot(pos_off);

  // when there's specular, ignore the diffuse
  diff_contrib *= (1 - spec_contrib);
  if (diff_contrib >= 0.0) {
    return light->color * light->intensity * diff_contrib;
  }
  return Color(0, 0, 0);
}

Color World::computeSpecular(const std::shared_ptr<Light>& light,
                             const Intersection& intersect,
                             float& spec_contrib, const Vector& sample_pos) {
  Color c(0.0, 0.0, 0.0);

  Vector ray_dir = (intersect.ray.origin - intersect.pt).normalize();
  Vector pos_off = (sample_pos - intersect.pt).normalize();
  Vector refl_vect = UTILreflectVector(pos_off, intersect.nml);

  // How much specular is contributed, proportional to the dot product
  // of the reverse incident ray direction and reflection vector.
  spec_contrib = ray_dir.dot(refl_vect);
  if (spec_contrib <= 0) {
    spec_contrib = 0;
    return c;
  }

  float shininess = 20.0;
  spec_contrib = pow(spec_contrib, shininess) * (shininess / 100.0);

  float amt = spec_contrib * light->intensity;
  c.r += amt;
  c.g += amt;
  c.b += amt;

  return c;
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
  if (intersect.ray.remaining_casts <= 0)
    return Color(0, 0, 0);

  Vector incident = intersect.pt - intersect.ray.origin;
  Ray r(intersect.pt,
        UTILreflectVector(incident, intersect.nml),
        intersect.ray.remaining_casts - 1);

  return traceRay(r) * RAY_CAST_ATTENUATION;
}

Color World::computeLighting(const Intersection& intersect) {
  Color diffuse(0.0, 0.0, 0.0);
  Color specular(0.0, 0.0, 0.0);

  for (int i = 0; i < (int)lightList.size(); i++) {
    float spec_contrib = 0;
    float diff_contrib = 0;

    // We do uniform random sampling on the surface area of this light source,
    // so the shadows will be soft.
    // In real life, point lights do not exist, so we get this for free. But in
    // computers... it's not so easy...
    int samples = lightList[i]->getNumSamples();

    Color cur_diff(0.0, 0.0, 0.0);
    Color cur_spec(0.0, 0.0, 0.0);
    for (int j = 0; j < samples; j++) {
      Vector sample_pos;
      lightList[i]->sample(sample_pos);
      if (!castShadowRay(sample_pos, intersect)) {
        cur_spec +=
            computeSpecular(lightList[i], intersect, spec_contrib, sample_pos);
        cur_diff += computeDiffuse(lightList[i], intersect, spec_contrib,
                                   diff_contrib, sample_pos);
      }
    }
    cur_diff /= (1.0 * samples);
    cur_spec /= (1.0 * samples);
    diffuse += cur_diff * intersect.object->sampleTexture(intersect.pt);
    specular += cur_spec;
  }

  return diffuse + specular + computeRefractiveReflective(intersect);
}
