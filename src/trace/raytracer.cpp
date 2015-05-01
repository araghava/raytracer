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

    // If this ray has intersected any objects.
    if (world.getClosestIntersection(ray, intersection))
        return world.computeLighting(intersection);

    return Color();
}

bool Raytracer::render(const std::string &outpath)
{
    Screen screen(renderParms.width, renderParms.height);
    int pxWidth = renderParms.width * renderParms.antialias;
    int pxHeight = renderParms.height * renderParms.antialias;
    int pxSmaller = std::min(pxWidth, pxHeight);

    float zoomFactor = pxSmaller;
    Vector camera(0.0, 0.0, 0.0);
    Vector direction(0.0, 0.0, -1.0);

    // TODO: parallelize
    for (int i = 0; i < renderParms.width; i++)
    {
        for (int j = 0; j < renderParms.height; j++)
        {
            int a_i = i * renderParms.antialias;
            int a_j = j * renderParms.antialias;

            Color avg(0.0, 0.0, 0.0);
            for (int ii = a_i; ii < a_i + renderParms.antialias; ii++)
            {
                direction.x = (ii - 0.5*pxWidth) / zoomFactor;
                for (int jj = a_j; jj < a_j + renderParms.antialias; jj++)
                {
                    direction.y = (jj - 0.5*pxHeight) / zoomFactor;
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
