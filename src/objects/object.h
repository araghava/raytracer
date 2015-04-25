#ifndef __OBJECT_H_
#define __OBJECT_H_

#include "../core/vector3.h"
#include "../trace/ray.h"

// Abstract class for an object.
class Object {
public:
             Object(const Vector &c_ = Vector()) :
                 center(c_),
                 opacity(1) {}
    virtual ~Object() {}

    // Intersects a ray with this object, and populates the Intersection struct
    // with the results. Returns true if there was an intersection.
    // TODO: acceleration data structure.
    virtual bool intersect(const Ray &ray, Intersection &intersection) = 0;

    // Check if this object contains the given point.
    virtual bool contains(const Vector &point) = 0;

    void setOpacity(float o)
    { opacity = o; }

    const Vector &getCenter()
    { return center; }
    void setCenter(const Vector &vec)
    { center = vec; }

protected:
    Vector center;
    float opacity;
};

#endif
