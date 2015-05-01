#ifndef __WORLD_H
#define __WORLD_H

#include <vector>
#include <stdio.h>

#include "ray.h"

#include "../objects/object.h"
#include "../core/color.h"

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

    // Computes lighting at a certain intersection.
    // Diffuse, specular, reflective, refractive components.
    Color computeLighting(const Intersection &intersect);
private:
    // Computes specular/diffuse contributions to an intersection from
    // a given light.
    Color computeDiffuse(Light *light,
                         const Intersection &intersect,
                         float spec_contrib,
                         float &diff_contrib);
    Color computeSpecular(Light *light,
                          const Intersection &intersect,
                          float &spec_contrib);
    bool castShadowRays(Light *light, const Intersection &intersect);

    std::vector<Object *> objectList;
    std::vector<Light *> lightList;
};

#endif
