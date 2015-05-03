#ifndef __WORLD_H
#define __WORLD_H

#include <vector>
#include <stdio.h>

#include "ray.h"

#include "../objects/object.h"
#include "../core/color.h"
#include "../core/util.h"

class World {
public:
            World();
           ~World();

    void addObject(Object *obj)
    { objectList.push_back(obj); }

    void addLight(Light *light)
    { lightList.push_back(light); }

    // Looking at every object, this finds the closest intersection.
    // TODO: spatial partitioning data structure for acceleration.
    bool getClosestIntersection(Ray &ray,
                                Intersection &intersect);

    Color traceRay(Ray &ray);
private:
    // Computes lighting at a certain intersection.
    // Diffuse, specular, reflective, refractive components.
    Color computeLighting(const Intersection &intersect);

    // Computes specular/diffuse contributions to an intersection from
    // a given light.
    Color computeDiffuse(Light *light,
                         const Intersection &intersect,
                         float spec_contrib,
                         float &diff_contrib,
                         const Vector &sample_pos);
    Color computeSpecular(Light *light,
                          const Intersection &intersect,
                          float &spec_contrib,
                          const Vector &sample_pos);

    // Computes refractive and reflective color contributions.
    Color computeRefractiveReflective(const Intersection &intersect);

    // Casts a shadow ray towards the sampled light position. If there's an object
    // between the light and the intersection, it returns true, if not, it returns false.
    bool castShadowRay(const Vector &sample_pos, const Intersection &intersect);

    std::vector<Object *> objectList;
    std::vector<Light *> lightList;
};

#endif
