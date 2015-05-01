#include "world.h"
#include <cstdlib>
#include <cfloat>
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
    float min_dist = FLT_MAX;
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

Color World::traceRay (Ray &ray)
{
    Intersection inter;
    if (getClosestIntersection(ray, inter))
        return computeLighting(inter);
    return Color();
}

Color World::computeDiffuse(Light *light,
                            const Intersection &intersect,
                            float spec_contrib,
                            float &diff_contrib,
                            const Vector &sample_pos)
{
    Color c(0.0, 0.0, 0.0);

    Vector pos_off = (sample_pos - intersect.pt).normalize();

    // The amount this light contributes is proportional to the dot
    // product of the relative light position and surface normal.
    diff_contrib = intersect.nml.dot(pos_off);

    // HACK: When there's specular, ignore the diffuse.
    diff_contrib *= (1 - spec_contrib);

    if (diff_contrib >= 0.0)
        c = light->color * (light->intensity * diff_contrib);
    return c;
}

Color World::computeSpecular(Light *light,
                             const Intersection &intersect,
                             float &spec_contrib,
                             const Vector &sample_pos)
{
    Color c(0.0, 0.0, 0.0);

    Vector ray_dir = (intersect.ray.origin - intersect.pt).normalize();
    Vector pos_off = (sample_pos - intersect.pt).normalize();
    Vector refl_vect = intersect.nml * 2 * pos_off.dot(intersect.nml) - pos_off;

    // How much specular is contributed, proportional to the dot product
    // of the reverse incident ray direction and reflection vector.
    spec_contrib = ray_dir.dot(refl_vect);
    if (spec_contrib <= 0)
    {
        spec_contrib = 0;
        return c;
    }

    float shininess = 25.0;
    spec_contrib = pow(spec_contrib, shininess) * (shininess / 100.0);

    float amt = spec_contrib * light->intensity;
    c.r += amt;
    c.g += amt;
    c.b += amt;

    return c;
}

bool World::castShadowRay(const Vector &position, const Intersection &intersect)
{
    Ray shadow_ray(intersect.pt, (position - intersect.pt).normalize(), 25);
    Intersection inter;
    float lightdist2 = (position - intersect.pt).length2();

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

        // We do uniform random sampling on the surface area of this light source,
        // so the shadows will be soft.
        // In real life, point lights do not exist, so we get this for free. But in
        // computers... it's not so easy...
        int samples = lightList[i]->getNumSamples();

        Color cur_diff(0.0, 0.0, 0.0);
        Color cur_spec(0.0, 0.0, 0.0);
        for (int j = 0; j < samples; j++)
        {
            Vector sample_pos;
            lightList[i]->sample(sample_pos);
            if (!castShadowRay(sample_pos, intersect))
            {
                cur_spec += computeSpecular(lightList[i],
                                intersect, spec_contrib, sample_pos);
                cur_diff += computeDiffuse(lightList[i],
                                intersect, spec_contrib, diff_contrib, sample_pos);
            }
        }

        cur_diff /= (1.0 * samples);
        cur_spec /= (1.0 * samples);
        diffuse += cur_diff;
        specular += cur_spec;
    }

    return diffuse + specular;
}
