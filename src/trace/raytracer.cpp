#include "raytracer.h"

#include "screen.h"
#include "../core/vector3.h"
#include "../core/color.h"

Raytracer::Raytracer(const RenderParms &parms)
    : renderParms(parms)
{
}

Raytracer::~Raytracer()
{
}

bool Raytracer::render(const std::string &outpath)
{
    // Oversample the image using the antialiasing factor.
    int pxWidth = renderParms.antialias * renderParms.width;
    int pxHeight = renderParms.antialias * renderParms.height;
    int minDim = std::min(pxWidth, pxHeight);

    // The eye originates at the origin.
    Vector origin(0.0, 0.0, 0.0);

    // Camera faces in the -z direction (RHR with +x at right, +y up).
    Vector direction(0.0, 0.0, -1.0);
    Screen screen(renderParms.width, renderParms.height);

    Color *colorBuffer = (Color *)malloc(pxWidth * pxHeight * sizeof(Color));
    float zoomFactor = renderParms.antialias * minDim;

    // TODO: parallelize
    for (int i = 0; i < pxWidth; i++)
    {
        direction.x = (i - pxWidth/2.0) / zoomFactor;
        for (int j = 0; j < pxHeight; j++)
        {
            direction.y = (pxHeight/2.0 - j) / zoomFactor;
            colorBuffer[i + j*pxWidth] = Color(1.0*i/pxWidth, 1.0*j/pxHeight, 0.);//tracePrimaryRay(origin, direction);
        }
    }

    // Handle antialiasing and write out the file.
    for (int i = 0; i < renderParms.width; i++)
    {
        for (int j = 0; j < renderParms.height; j++)
        {
            Color sum(0.0, 0.0, 0.0);
            for (int ii = 0; ii < renderParms.antialias; ii++)
            {
                for (int jj = 0; jj < renderParms.antialias; jj++)
                {
                    sum += colorBuffer[
                        (i*renderParms.antialias + ii) +
                        pxWidth * (j * renderParms.antialias) + jj
                    ];
                }
            }
            sum /= 1.0*(renderParms.antialias * renderParms.antialias);
            screen.setPixel(i, j, sum);
        }
    }

    free(colorBuffer);
    screen.writeImage(outpath.c_str());
    return true;
}
