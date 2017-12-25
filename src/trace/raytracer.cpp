#include "raytracer.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include "../core/geometry.h"
#include "../core/color.h"
#include "../core/constants.h"

Raytracer::Raytracer(const int nt) : numThreads(nt) {
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
  int pxWidth = p->raytracer->getCamera()->width * p->raytracer->getCamera()->antialias;
  int pxHeight = p->raytracer->getCamera()->height * p->raytracer->getCamera()->antialias;
  int pxSmaller = std::min(pxWidth, pxHeight);

  float zoomFactor = pxSmaller;
  Vector camera = p->raytracer->getCamera()->pos;
  Vector direction = p->raytracer->getCamera()->dir;

  for (int i = p->start_row; i <= p->end_row; i++) {
    for (int j = 0; j < p->raytracer->getCamera()->height; j++) {
      int a_i = i * p->raytracer->getCamera()->antialias;
      int a_j = j * p->raytracer->getCamera()->antialias;

      // For each pixel, shoot aa*aa primary rays, where aa is the antialiasing
      // factor. Average the results of these color values.
      Color avg(0.0, 0.0, 0.0);
      for (int ii = a_i; ii < a_i + p->raytracer->getCamera()->antialias; ii++) {
        direction.x = (ii - 0.5 * pxWidth) / zoomFactor;
        for (int jj = a_j; jj < a_j + p->raytracer->getCamera()->antialias; jj++) {
          direction.y = (jj - 0.5 * pxHeight) / zoomFactor;
          direction.normalize();
          avg += p->raytracer->tracePrimaryRay(camera, direction);
        }
      }
      avg /= (1.0 * p->raytracer->getCamera()->antialias * p->raytracer->getCamera()->antialias);
      (*p->buffer)[i][j] = avg;
    }

    p->progressReporter->report();
  }

  delete p;
}

bool Raytracer::render(const std::string& outpath) {
  // Split up the work to be done based on how many threads are available.
  const int num_threads = std::max(numThreads, 1);
  const int chunkSize = ceil(1.0 * camera->width / num_threads);
  std::thread threads[num_threads];

	std::vector<std::vector<Color>> buffer;
	for (int i = 0; i < camera->width; i++) {
		buffer.push_back(std::vector<Color>(camera->height));
	}

	auto progressReporter = std::make_shared<ProgressReporter>(camera->width);

  for (int i = 0; i < num_threads; i++) {
    const int start = i * chunkSize;
    const int end = std::min(camera->width - 1, start + chunkSize - 1);
    RaytraceThreadParms *parms = new RaytraceThreadParms();
    parms->raytracer = this;
    parms->start_row = start;
    parms->end_row = end;
    parms->buffer = &buffer;
    parms->progressReporter = progressReporter;

    threads[i] = std::thread(raytrace_threading_fn, parms);
  }

  // Wait until all the pixel values have been computed.
  for (int i = 0; i < num_threads; i++) {
    threads[i].join();
  }

  progressReporter->finalize();

	Screen screen(camera->width, camera->height);
  for (int i = 0; i < camera->width; i++) {
    for (int j = 0; j < camera->height; j++) {
      screen.setPixel(i, j, buffer[i][j]);
		}
	}

  screen.writeImage(outpath.c_str());
  return true;
}
