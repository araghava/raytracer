#ifndef __OBJECT_H
#define __OBJECT_H

#include "../core/vector3.h"
#include "../core/color.h"
#include "../trace/ray.h"

struct Intersection;
class Ray;

// Abstract Object implementation.
class Object {
public:
             Object(const Vector &c)
             { setCenter(c); }
    virtual ~Object() {}

    Vector &getCenter()
    { return center; }
    void    setCenter(const Vector &v)
    { center = v; }

    // Methods to be overriden by subclasses.
    virtual bool intersect(const Ray &ray,
                           Intersection &intersection) = 0;
    virtual bool contains(const Vector &point) = 0;
private:
    Vector center;
};

class Light {
public:
            Light(const Vector &p, Color c, float inten)
                : position(p)
                , color(c)
                , intensity(inten) {}
           ~Light() {}
    Vector position;
    Color color;
    float intensity;
};

#endif
