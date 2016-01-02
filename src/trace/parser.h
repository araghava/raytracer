#ifndef __PARSER_H
#define __PARSER_H

#include <string>
#include <istream>

#include "raytracer.h"

class Parser {
public:
  Parser() {}
  bool createWorld(Raytracer *tracer, std::string &errors, std::istream &is);

private:
  bool parseObjects(Raytracer *tracer, int oc, std::istream &is);
  bool parseLights(Raytracer *tracer, int lc, std::istream &is);
  bool parseParameters(Raytracer *tracer, int pc, std::istream &is);
};

#endif
