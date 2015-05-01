#include "raytracer.h"

#include "../core/vector3.h"
#include "../core/color.h"

Raytracer::Raytracer(const RenderParms &parms, const Camera &camera)
    : renderParms(parms)
    , cam(camera)
{
}

Raytracer::~Raytracer()
{
}

void Raytracer::addObject(Object *obj)
{
    world.addObject(obj);
}

void Raytracer::addLight(Light *light)
{
    world.addLight(light);
}

Color Raytracer::tracePrimaryRay(const Vector &origin,
                                 const Vector &direction)
{
    Intersection intersection;
    Ray ray(origin, direction, 25);

    return world.traceRay(ray);
}

bool Raytracer::render(const std::string &outpath)
{
    Screen screen(renderParms.width, renderParms.height);
    int pxWidth = renderParms.width * renderParms.antialias;
    int pxHeight = renderParms.height * renderParms.antialias;
    int pxSmaller = std::min(pxWidth, pxHeight);

    float zoomFactor = pxSmaller;

    // TODO: try using pthreads for multithreading, it might be faster than OpenMP
    #pragma omp parallel for
    for (int i = 0; i < renderParms.width; i++)
    {
        // Camera position starts at the origin, direction is in -z.
        Vector camera(0.0, 0.0, 0.0);
        Vector direction(0.0, 0.0, -1.0);

        for (int j = 0; j < renderParms.height; j++)
        {
            int a_i = i * renderParms.antialias;
            int a_j = j * renderParms.antialias;

            // For each pixel, shoot aa*aa primary rays, where aa is the antialiasing
            // factor. Average the results of these color values.
            Color avg(0.0, 0.0, 0.0);
            for (int ii = a_i; ii < a_i + renderParms.antialias; ii++)
            {
                direction.x = (ii - 0.5*pxWidth) / zoomFactor;
                for (int jj = a_j; jj < a_j + renderParms.antialias; jj++)
                {
                    direction.y = (jj - 0.5*pxHeight) / zoomFactor;
                    direction.normalize();
                    avg += tracePrimaryRay(camera, direction);
                }
            }
            avg /= (1.0 * renderParms.antialias * renderParms.antialias);
            screen.setPixel(i, j, avg);
        }
    }

    screen.writeImage(outpath.c_str());
    return true;
}
