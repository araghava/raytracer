#include "world.h"
#include <cstdlib>
#include <math.h>

World::World()
{
}

World::~World()
{
    for (int i = 0; i < (int)objectList.size(); i++)
        delete objectList[i];
    for (int i = 0; i < (int)lightList.size(); i++)
        delete lightList[i];
}

bool World::getClosestIntersection(Ray &ray,
                                   Intersection &intersect)
{
    float min_dist = 1e6;
    bool found = false;

    // Check each object for intersection.
    for (int i = 0; i < (int)objectList.size(); i++) {
        Intersection inter;
        if (objectList[i]->intersect(ray, inter))
        {
            found = true;
            float dist = (inter.pt - ray.origin).length2();

            // If this is closer, record this one.
            if (dist < min_dist)
            {
                min_dist = dist;
                intersect = inter;
            }
        }
    }

    return found;
}

Color World::computeDiffuse(Light *light, const Intersection &intersect)
{
    Vector pos_off = (light->position - intersect.pt).normalize();

    // The amount this light contributes is proportional to the dot
    // product of the relative light position and surface normal.
    float contribution = intersect.nml.dot(pos_off);
    if (contribution >= 0.0)
        return light->color * (light->intensity * contribution);

    return Color();
}

Color World::computeSpecular(Light *light, const Intersection &intersect)
{
    Color c(0.0, 0.0, 0.0);

    Vector ray_dir = (intersect.ray.origin - intersect.pt).normalize();
    Vector pos_off = (light->position - intersect.pt).normalize();
    Vector refl_vect = intersect.nml * 2 * pos_off.dot(intersect.nml) - pos_off;

    // How much specular is contributed, proportional to the dot product
    // of the reverse incident ray direction and reflection vector.
    float contrib = ray_dir.dot(refl_vect);
    if (contrib <= 0)
        return c;
    contrib = pow(contrib, 3);
    float amt = contrib * light->intensity;
    c.r = amt;
    c.g = amt;
    c.b = amt;
    return c;
}

Color World::computeLighting(const Intersection &intersect)
{
    Color diffuse(0.0, 0.0, 0.0);
    Color specular(0.0, 0.0, 0.0);

    for (int i = 0; i < (int)lightList.size(); i++)
    {
        diffuse += computeDiffuse(lightList[i], intersect);
        specular += computeSpecular(lightList[i], intersect);
    }
    return diffuse + specular;
}
