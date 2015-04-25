#include "sphere.h"

Sphere::~Sphere()
{
}

bool Sphere::contains(const Vector &point)
{
    float r = radius + 1e-6;
    return (point - getCenter()).length2() <= r*r;
}

bool Sphere::intersect(const Ray &ray, Intersection &intersection)
{
    return true;
}
