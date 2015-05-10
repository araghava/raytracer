#ifndef __RAY_TRACER_H
#define __RAY_TRACER_H

#include <vector>
#include <string>

#include "../objects/object.h"
#include "world.h"
#include "screen.h"

class Raytracer;

struct RenderParms {
    // Width and height, in pixels.
    int width = 1024;
    int height = 1024;

    // Default to 4x4 antialiasing (split each pixel into 16 and average).
    int antialias = 2;
};

// Threading structure for raytracing.
struct RaytraceThreadParms {
    Raytracer *raytracer;
    Screen *screen;
    const RenderParms *parms;

    int start_row;
    int end_row;

    Color **buffer;
};

class Raytracer {
public:
            Raytracer();
           ~Raytracer();

    // Caller must allocate memory for the object, this class will
    // then take ownership.
    void addObject(Object *object);

    // Caller must allocate memory for the object, this class will
    // then take ownership.
    void addLight(Light *light);
    void setParms(RenderParms parms)
    { renderParms = parms; }

    bool render(const std::string &out_path);

    // Fires primary rays. These are rays that originate at the camera.
    Color tracePrimaryRay(const Vector &origin,
                          const Vector &direction);
private:

    RenderParms renderParms;

    std::vector<Object *> objectList;
    std::vector<Light *> lightList;

    World world;
};

#endif
