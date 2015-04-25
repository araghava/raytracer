#ifndef __RAY_H_
#define __RAY_H_

#include "../core/vector3.h"

// To avoid ray intersections that occur at the exact same spot repeatedly,
// we add a little bias at the ray origin.
static const int RAY_INTERSECTION_BIAS = 1e-4;

class Ray {
public:
    Ray() : origin(Vector()),
            direction(Vector()),
            remaining_casts(-1)
    {
        origin += RAY_INTERSECTION_BIAS;
        direction.normalize();
    }
    Ray(Vector o, Vector d, int r) :
        origin(o), direction(d), remaining_casts(r)
    {
        origin += RAY_INTERSECTION_BIAS;
        direction.normalize();
    }

    const Vector &getOrigin()
    { return origin; }
    const Vector &getDirection()
    { return origin; }
private:
    // Origin and direction that defines this ray.
    Vector origin;
    Vector direction;

    // How many more times can this ray be casted (reflected/refracted).
    int remaining_casts;
};

struct Intersection {
    Ray ray;
    //Object *object;
    Vector pt, nml;
    bool done;
};

#endif
