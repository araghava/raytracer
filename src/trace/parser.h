#ifndef __PARSER_H
#define __PARSER_H

#include "../objects/texture.h"
#include "raytracer.h"

#include <istream>
#include <memory>
#include <string>
#include <unordered_map>

class Parser {
public:
  Parser();

  bool createWorld(Raytracer *tracer, std::string &errors, std::istream &is);

private:
  std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;

  bool parseObjects(Raytracer *tracer, int oc, std::istream &is);
  bool parseLights(Raytracer *tracer, int lc, std::istream &is);
  bool parseParameters(Raytracer *tracer, int pc, std::istream &is);
  bool parseTextures(Raytracer *tracer, int tc, std::istream &is);
};

#endif
