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

Color World::computeDiffuse(Light *light,
                            const Intersection &intersect,
                            float spec_contrib,
                            float &diff_contrib)
{
    Vector pos_off = (light->position - intersect.pt).normalize();

    // The amount this light contributes is proportional to the dot
    // product of the relative light position and surface normal.
    diff_contrib = intersect.nml.dot(pos_off);

    // HACK: When there's specular, ignore the diffuse.
    diff_contrib *= (1 - spec_contrib);

    if (diff_contrib >= 0.0)
        return light->color * (light->intensity * diff_contrib);

    return Color();
}

Color World::computeSpecular(Light *light,
                             const Intersection &intersect,
                             float &spec_contrib)
{
    Color c(0.0, 0.0, 0.0);

    Vector ray_dir = (intersect.ray.origin - intersect.pt).normalize();
    Vector pos_off = (light->position - intersect.pt).normalize();
    Vector refl_vect = intersect.nml * 2 * pos_off.dot(intersect.nml) - pos_off;

    // How much specular is contributed, proportional to the dot product
    // of the reverse incident ray direction and reflection vector.
    spec_contrib = ray_dir.dot(refl_vect);
    if (spec_contrib <= 0)
    {
        spec_contrib = 0;
        return c;
    }
    spec_contrib = pow(spec_contrib, 55);
    float amt = spec_contrib * light->intensity;
    c.r = amt;
    c.g = amt;
    c.b = amt;
    return c;
}

bool World::castShadowRays(Light *light, const Intersection &intersect)
{
    // TODO: use randomized sampling.
    Ray shadow_ray(intersect.pt, (light->position - intersect.pt).normalize(), 25);
    Intersection inter;
    float lightdist2 = (light->position - intersect.pt).length2();

    if (getClosestIntersection(shadow_ray, inter))
    {
        float dist2 = (inter.pt - intersect.pt).length2();
        if (dist2 < lightdist2)
            return true;
    }

    return false;
}

Color World::computeLighting(const Intersection &intersect)
{
    Color diffuse(0.0, 0.0, 0.0);
    Color specular(0.0, 0.0, 0.0);

    for (int i = 0; i < (int)lightList.size(); i++)
    {
        float spec_contrib;
        float diff_contrib;

        // If the light rays can actually reach the intersection (it's not
        // blocked by an object), add the contribution.
        if (!castShadowRays(lightList[i], intersect))
        {
            specular += computeSpecular(lightList[i], intersect, spec_contrib);
            diffuse += computeDiffuse(lightList[i], intersect, spec_contrib, diff_contrib);
        }
    }
    return diffuse + specular;
}
