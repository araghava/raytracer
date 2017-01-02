#ifndef __XMLPARSER_H
#define __XMLPARSER_H

#include <string>

#include "screen.h"
#include "raytracer.h"
#include "../external/tinyxml2.h"
#include "../material/material.h"

class XMLParser {
 public:
  XMLParser(Raytracer* const rt, const std::string& file);

  bool parse();

 private:
  bool initializeCamera(tinyxml2::XMLElement* const camera);
  bool initializeScene(tinyxml2::XMLElement* const scene);
  bool initializeObject(tinyxml2::XMLElement* const object);
  bool initializeLight(tinyxml2::XMLElement* const light);
  bool initializeMaterial(tinyxml2::XMLElement* const material);

  Raytracer* const tracer;
  const std::string fileName;

  std::unordered_map<size_t, std::string> objectMaterialMap;
  std::unordered_map<std::string, std::shared_ptr<Material>> materialMap;;
  std::vector<std::shared_ptr<Object>> objectList;
};

#endif
