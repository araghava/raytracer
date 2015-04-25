#ifndef __RAY_TRACER_H
#define __RAY_TRACER_H

#include <vector>
#include <string>

struct RenderParms {
    // Width and height, in pixels.
    int width = 300;
    int height = 300;

    // 2x2 antialiasing (split each pixel into four and average).
    int antialias = 2;
};

class Raytracer {
public:
            Raytracer(const RenderParms &parms);
           ~Raytracer();
/*
    void addObject(Object *object);
    void addLight(Light *light);
*/
    bool render(const std::string &out_path);

private:
    const RenderParms &renderParms;
};

#endif
