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

    RenderParms parms;
    Raytracer tracer(parms);
    std::string out = "images/out.tga";

    tracer.render(out);
    return 0;
}
