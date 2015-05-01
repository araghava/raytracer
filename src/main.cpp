#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

#include "core/color.h"
#include "core/vector3.h"
#include "trace/ray.h"
#include "trace/raytracer.h"
#include "trace/screen.h"
#include "objects/sphere.h"
#include "objects/plane.h"

void usage()
{
    printf("some usage...\n");
}

int main(int argc, char **argv)
{
    /*
    if (argc <= 1)
    {
        usage();
        return 0;
    }*/

    Camera cam;
    RenderParms parms;

    Raytracer tracer(parms, cam);
    std::string out = "images/out.tga";

    tracer.addLight(new Light(Vector(-5, 4, 3), Color(0, 0, 1), 0.5));
    tracer.addObject(new Sphere(Vector(0, 0, -3), 1));
    tracer.addObject(new Plane(Vector(0, -2, -1),
                               Vector(-1, -2, 1),
                               Vector(1, -2, 1)));
    tracer.render(out);
    return 0;
}
