#include "sphere.h"
#include <cstdio>
#include <math.h>

Sphere::~Sphere()
{
}

bool Sphere::contains(const Vector &point)
{
    float r = radius + 1e-6;
    return (point - getCenter()).length2() <= r*r;
}

bool Sphere::intersect(const Ray &ray,
                       Intersection &intersection)
{
    Vector origin = ray.origin;
    Vector direction = ray.direction;
    direction.normalize();

    // Calculate the coefficients of the quadratic equation
    // au^2 + bu + c = 0. We seek the value of u for each intersection.
    Vector displace = origin - getCenter();
    float a = direction.dot(direction);
    float b = 2.0 * displace.dot(direction);
    float c = displace.dot(displace) - radius*radius;
    float radicand = b*b-4*a*c;

    if (radicand < 0.0)
        return false;

    radicand = sqrtf(radicand);
    float r1 = (-b - radicand)*0.5;
    float r2 = (-b + radicand)*0.5;

    float dist = r1;
    if (dist < 0) dist = r2;
    if (dist < 0)
        return false;

    Vector pt = origin + (direction*dist);
    Vector nml = (pt - getCenter()).normalize();

    if (direction.dot(nml) > 0)
        nml *= -1;

    intersection.object = this;
    intersection.pt = pt;
    intersection.nml = nml;
    return true;
}
