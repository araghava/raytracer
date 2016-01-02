#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <istream>
#include <fstream>

#include "trace/raytracer.h"
#include "trace/parser.h"

static const char *DEFAULT_IMAGE_OUT = "../images/out.tga";

void usage() {
  printf("\nRaytracer\n");
  printf("    Reads scene from a file, sample scenes are in ../scenes/\n");
  printf("    Parameters:\n");
  printf("        -o    output file, defaults to ../images/out.tga\n");
  printf("        -s    scene file. if not provided it reads from standard "
         "input\n");

  printf("\n");
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    usage();
    return 0;
  }

  char *scene_file = 0;
  char *out_file = 0;
  int c;

  std::ifstream scene;
  while ((c = getopt(argc, argv, "o:s:")) != -1) {
    switch (c) {
    case 'o':
      out_file = optarg;
      break;
    case 's':
      scene_file = optarg;
      break;
    default:
      abort();
    }
  }

  Raytracer tracer;
  Parser parser;
  std::string err, out;

  if (scene_file) {
    std::ifstream scene;
    scene.open(scene_file, std::ifstream::in);

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
  if (out_file)
    out = out_file;

  scene.close();
  tracer.render(out);
  return 0;
}
