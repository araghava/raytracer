#ifndef __PARSER_H
#define __PARSER_H

#include "../texture/texture.h"
#include "raytracer.h"

#include <istream>
#include <memory>
#include <string>
#include <unordered_map>

class Parser {
public:
  Parser();

  bool createWorld(Raytracer* const tracer, std::string& errors, std::istream &is);

private:
  std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;

  bool parseObjects(Raytracer* const tracer, const int oc, std::istream &is);
  bool parseLights(Raytracer* const tracer, const int lc, std::istream &is);
  bool parseParameters(Raytracer* const tracer, const int pc, std::istream &is);
  bool parseTextures(Raytracer* const tracer, const int tc, std::istream &is);
};

#endif
