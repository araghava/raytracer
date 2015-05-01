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
    std::string out = "../images/out.tga";

    tracer.addLight(new SphereLight(Vector(-1.5, 2, -2), Color(1, 1, 1), 0.5, 0.20));
//    tracer.addLight(new SphereLight(Vector(0, 2, -2), Color(1,1,1), 1, 0.25));
//    tracer.addLight(new SphereLight(Vector(-1.5, 2, -2), Color(1, 0, 0), 0.5, 0.1));
//    tracer.addLight(new SphereLight(Vector(0, 5, -12), Color(1, 1, 1), 0.4, 0.5));
    tracer.addObject(new Sphere(Vector(-1, 0, -4), 0.5));
    tracer.addObject(new Sphere(Vector(1, 0, -3), 0.5));
//    tracer.addObject(new Sphere(Vector(1, 0, -60), 25));
    tracer.addObject(new Plane(Vector(0, -0.5, -1), Vector(-1, -0.5, 1), Vector(1, -0.5, 1)));
    tracer.addObject(new Plane(Vector(1, 0, -10), Vector(-1, 0, -10), Vector(0, 1, -10)));
    tracer.render(out);
    return 0;
}
