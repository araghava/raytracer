#ifndef __RAY_TRACER_H
#define __RAY_TRACER_H

#include <string>
#include <thread>
#include <vector>
#include <unordered_map>

#include "../core/progress.h"
#include "../objects/object.h"
#include "world.h"
#include "screen.h"

class Raytracer;

struct RenderParms {
  // Width and height, in pixels.
  int width = 1024;
  int height = 1024;

  // Default to 4x4 antialiasing (split each pixel into 16 and average).
  int antialias = 2;
};

// Threading structure for raytracing.
struct RaytraceThreadParms {
  Raytracer* raytracer;
  Screen* screen;

  const RenderParms* parms;
  ProgressReporter* progressReporter;

  int start_row;
  int end_row;

  Color** buffer;
};

class Raytracer {
public:
  Raytracer(const int nt = std::thread::hardware_concurrency());
  ~Raytracer() = default;

  void addObject(std::shared_ptr<Object>& obj);
  void addLight(std::shared_ptr<Light>& light);
  void setCamera(std::shared_ptr<Camera>& cam) {
    camera = cam;
  }
  std::shared_ptr<Camera> getCamera() {
    return camera;
  }

  bool render(const std::string& out_path);

  // Fires primary rays. These are rays that originate at the camera.
  Color tracePrimaryRay(const Vector& origin, const Vector& direction);

private:
  std::vector<std::shared_ptr<Object>> objectList;
  std::vector<std::shared_ptr<Light>> lightList;

  World world;

  const int numThreads;

  std::shared_ptr<Camera> camera;
};

#endif
