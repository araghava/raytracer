#ifndef __RAY_TRACER_H
#define __RAY_TRACER_H

#include <vector>
#include <string>

#include "../objects/object.h"
#include "world.h"
#include "screen.h"

struct RenderParms {
    // Width and height, in pixels.
    int width = 512;
    int height = 512;

    // Default to 4x4 antialiasing (split each pixel into 16 and average).
    int antialias = 4;
};

class Raytracer {
public:
            Raytracer(const RenderParms &parms, const Camera &cam);
           ~Raytracer();

    // Caller must allocate memory for the object, this class will
    // then take ownership.
    void addObject(Object *object);

    // Caller must allocate memory for the object, this class will
    // then take ownership.
    void addLight(Light *light);

    bool render(const std::string &out_path);
private:
    // Fires primary rays. These are rays that originate at the camera.
    Color tracePrimaryRay(const Vector &origin,
                          const Vector &direction);

    const RenderParms &renderParms;
    const Camera &cam;

    std::vector<Object *> objectList;
    std::vector<Light *> lightList;

    World world;
};

#endif
