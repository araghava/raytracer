#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <istream>
#include <fstream>

#include "trace/raytracer.h"
#include "trace/parser.h"

namespace {
  const std::string DEFAULT_IMAGE_OUT = "../images/out.tga";
  const std::string DEFAULT_SCENE = "../scenes/reflection.scene";
}

void usage() {
  std::cout << "\nRaytracer\n";
  std::cout << "    Reads scene from a file, sample scenes are in ../scenes/\n";
  std::cout << "    Parameters:\n";
  std::cout << "        -o    output file, defaults to ../images/out.tga\n";
  std::cout << "        -s    scene file. if not provided it reads from standard input\n";
  std::cout << std::endl;
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    usage();
    return 0;
  }

  std::string scene_file;
  std::string out_file;
  int c, threads = 0;

  std::ifstream scene;
  while ((c = getopt(argc, argv, "o:s:t:")) != -1) {
    switch (c) {
    case 'o':
      out_file = std::string(optarg);
      break;
    case 's':
      scene_file = std::string(optarg);
      break;
    case 't':
      threads = std::atoi(optarg);
      break;
    default:
      abort();
    }
  }

  Raytracer tracer(threads <= 0 ? std::thread::hardware_concurrency() : threads);
  Parser parser;
  std::string err, out;

  if (!scene_file.empty()) {
    std::ifstream scene;
    scene.open(scene_file.c_str(), std::ifstream::in);

    if (!parser.createWorld(&tracer, err, scene)) {
      std::cout << err << "\n";
      return 1;
    }
    scene.close();
  } else {
    if (!parser.createWorld(&tracer, err, std::cin)) {
      std::cout << err << "\n";
      return 1;
    }
  }

  out = DEFAULT_IMAGE_OUT;
  if (out_file.empty()) {
    out = out_file;
  }

  scene.close();
  tracer.render(out.c_str());
  return 0;
}
