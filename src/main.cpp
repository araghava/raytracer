#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

#include "core/color.h"
#include "core/vector3.h"
#include "trace/ray.h"
#include "trace/raytracer.h"
#include "trace/screen.h"
#include "objects/sphere.h"

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

    tracer.addLight(new Light(Vector(-5, 5, -3), Color(1, 0, 1), 0.8));
    tracer.addObject(new Sphere(Vector(0, 0, -10), 2));
    tracer.render(out);
    return 0;
}
