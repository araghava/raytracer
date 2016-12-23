#include "raytracer.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include "../core/vector3.h"
#include "../core/color.h"
#include "../core/constants.h"

Raytracer::Raytracer(const int nt) : numThreads(nt) {
  RenderParms p;
  setParms(p);
}

void Raytracer::addObject(std::shared_ptr<Object>& obj) {
  world.addObject(obj);
}

void Raytracer::addLight(std::shared_ptr<Light>& light) {
  world.addLight(light);
}

Color Raytracer::tracePrimaryRay(const Vector& origin,
                                 const Vector& direction) {
  Intersection intersection;
  Ray ray(origin, direction, RAY_RECURSION_LIMIT);

  return world.traceRay(ray);
}

void raytrace_threading_fn(RaytraceThreadParms* p) {
  int pxWidth = p->parms->width * p->parms->antialias;
  int pxHeight = p->parms->height * p->parms->antialias;
  int pxSmaller = std::min(pxWidth, pxHeight);

  float zoomFactor = pxSmaller;
  Vector camera(0.0, 0.0, 0.0);
  Vector direction(0.0, 0.0, -1.0);

  for (int i = p->start_row; i <= p->end_row; i++) {
    for (int j = 0; j < p->parms->height; j++) {
      int a_i = i * p->parms->antialias;
      int a_j = j * p->parms->antialias;

      // For each pixel, shoot aa*aa primary rays, where aa is the antialiasing
      // factor. Average the results of these color values.
      Color avg(0.0, 0.0, 0.0);
      for (int ii = a_i; ii < a_i + p->parms->antialias; ii++) {
        direction.x = (ii - 0.5 * pxWidth) / zoomFactor;
        for (int jj = a_j; jj < a_j + p->parms->antialias; jj++) {
          direction.y = (jj - 0.5 * pxHeight) / zoomFactor;
          direction.normalize();
          avg += p->raytracer->tracePrimaryRay(camera, direction);
        }
      }
      avg /= (1.0 * p->parms->antialias * p->parms->antialias);
      p->buffer[i][j] = avg;
    }

    p->progressReporter->report();
  }

  delete p;
}

bool Raytracer::render(const std::string& outpath) {
  ProgressReporter progressReporter(renderParms.width);

  int num_threads = std::max(numThreads, 1);
  Screen screen(renderParms.width, renderParms.height);

  // Split up the work to be done based on how many threads are available.
  int chunkSize = renderParms.width / num_threads;
  std::thread threads[num_threads];

  // Hold a color buffer here so that we don't have thread competition for
  // screen.setPixel.
  // After we join all the threads, write out the image.
  Color **buffer = new Color *[renderParms.width];
  for (int i = 0; i < renderParms.width; i++)
    buffer[i] = new Color[renderParms.height];

  for (int i = 0; i < num_threads; i++) {
    const int start = i * chunkSize;
    const int end = std::min(renderParms.width - 1, start + chunkSize - 1);
    RaytraceThreadParms *parms = new RaytraceThreadParms();
    parms->raytracer = this;
    parms->parms = &renderParms;
    parms->start_row = start;
    parms->end_row = end;
    parms->buffer = buffer;
    parms->screen = &screen;
    parms->progressReporter = &progressReporter;

    threads[i] = std::thread(raytrace_threading_fn, parms);
  }

  // Wait until all the pixel values have been computed.
  for (int i = 0; i < num_threads; i++) {
    threads[i].join();
  }

  progressReporter.finalize();

  for (int i = 0; i < renderParms.width; i++)
    for (int j = 0; j < renderParms.height; j++)
      screen.setPixel(i, j, buffer[i][j]);

  // Free temp buffer memory.
  for (int i = 0; i < renderParms.width; i++)
    delete[] buffer[i];
  delete[] buffer;

  screen.writeImage(outpath.c_str());
  return true;
}
