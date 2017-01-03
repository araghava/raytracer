#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <istream>
#include <fstream>

#include "trace/xmlparser.h"
#include "trace/raytracer.h"

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
  std::cout << "        -t    threads. if not provided it uses thread::hardware_concurrency()\n";
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

  if (scene_file.empty()) {
    std::cout << "Please provide a scene file" << std::endl;
    return 1;
  }

  Raytracer tracer(threads <= 0 ? std::thread::hardware_concurrency() : threads);
  std::string err, out;

  XMLParser parser(&tracer, scene_file);
  parser.parse();

  out = DEFAULT_IMAGE_OUT;
  if (out_file.empty()) {
    out = out_file;
  }

  tracer.render(out.c_str());
  return 0;
}
