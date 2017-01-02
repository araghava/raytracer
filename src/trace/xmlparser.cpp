#include "xmlparser.h"

#include "../objects/objobject.h"
#include "../objects/primitives/plane.h"
#include "../objects/primitives/sphere.h"

#include <iostream>
#include <math.h>
#include <memory>

#define PARSE_ERROR(msg)           \
  {                                \
    std::cout << msg << std::endl; \
    exit(1);                       \
  }

namespace {
  Vector parseVectorItem(
    tinyxml2::XMLElement* const node,
    const std::string& xs = "x", const std::string& ys = "y", const std::string& zs = "z") {
    double x = 0, y = 0, z = 0;
    node->QueryDoubleAttribute(xs.c_str(), &x);
    node->QueryDoubleAttribute(ys.c_str(), &y);
    node->QueryDoubleAttribute(zs.c_str(), &z);
    return Vector(x, y, z);
  }

  int parseIntAttribute(
    tinyxml2::XMLElement* const node, const std::string& attribute) {
    int ret;
    node->QueryIntAttribute(attribute.c_str(), &ret);
    return ret;
  }

  float parseFloatAttribute(
    tinyxml2::XMLElement* const node, const std::string& attribute) {
    double ret = 0;
    node->QueryDoubleAttribute(attribute.c_str(), &ret);
    return ret;
  }

  std::string parseStringAttribute(
    tinyxml2::XMLElement* const node, const std::string& attribute) {
    const char* ret = node->Attribute(attribute.c_str());
    return ret ? std::string(ret) : "";
  }
}

XMLParser::XMLParser(
  Raytracer* const rt, const std::string& file) : tracer(rt), fileName(file) {
}

bool XMLParser::parse() {
  tinyxml2::XMLDocument doc(fileName.c_str());
  if (doc.LoadFile(fileName.c_str())){
    PARSE_ERROR("Failed to load the fileName '" << fileName << "'");
  }

  auto* xml = doc.FirstChildElement("xml");
  if (!xml) {
    PARSE_ERROR("No XML tag");
  }

  auto* camera = xml->FirstChildElement("camera");
  if (!camera || !initializeCamera(camera)) {
    PARSE_ERROR("Failed to initialize the camera");
  }

  // load objects, materials, lights
  auto* scene = xml->FirstChildElement("scene");
  if (!scene || !initializeScene(scene)) {
    PARSE_ERROR("Failed to initialize the scene");
  }

  for (size_t i = 0; i < objectList.size(); i++) {
    objectList[i]->load();

    if (objectMaterialMap.count(i)) {
      objectList[i]->setMaterial(materialMap[objectMaterialMap[i]]);
    }
    tracer->addObject(objectList[i]);
  }
  return true;
}

bool XMLParser::initializeCamera(tinyxml2::XMLElement* const camera) {
  auto* child = camera->FirstChildElement();
  auto c = std::make_shared<Camera>();

  while (child) {
    const auto& item = std::string(child->Value());
    if (item == "position") {
      c->pos = parseVectorItem(child);
    } else if (item == "direction") {
      c->dir = parseVectorItem(child);
    } else if (item == "up") {
      c->up = parseVectorItem(child);
    } else if (item == "resolution") {
      c->width = parseIntAttribute(child, "width");
      c->height = parseIntAttribute(child, "height");
      c->antialias = parseIntAttribute(child, "antialias");
      if (c->antialias == 0) {
        c->antialias = 2;
      }
    } else {
      PARSE_ERROR("Unknown camera field: " + item);
    }

    child = child->NextSiblingElement();
  }

  tracer->setCamera(c);
  return true;
}

bool XMLParser::initializeScene(tinyxml2::XMLElement* const scene) {
  auto* child = scene->FirstChildElement();

  while (child) {
    const auto& item = std::string(child->Value());
    if (item == "object" && !initializeObject(child)) {
      PARSE_ERROR("Unable to initialize object");
    } else if (item == "light" && !initializeLight(child)) {
      PARSE_ERROR("Unable to initialize light");
    } else if (item == "material" && !initializeMaterial(child)) {
      PARSE_ERROR("Unable to initialize material");
    }
    child = child->NextSiblingElement();
  }
  return true;
}

bool XMLParser::initializeObject(tinyxml2::XMLElement* const object) {
  std::shared_ptr<Object> obj;
  const auto type = parseStringAttribute(object, "type");
  const auto id = objectList.size();

  // create object based on type and attributes
  if (type == "sphere") {
    obj = std::make_shared<Sphere>(id, parseFloatAttribute(object, "radius"));
  } else if (type == "plane") {
    obj = std::make_shared<Plane>(id,
      parseVectorItem(object, "x1", "y1", "z1"),
      parseVectorItem(object, "x2", "y2", "z2"),
      parseVectorItem(object, "x3", "y3", "z3"));
  } else if (type == "obj") {
    obj = std::make_shared<ObjObject>(id, parseStringAttribute(object, "filename"));
  } else {
    std::cout << "Unknown object type: " << type << std::endl;
    return false;
  }

  // parse object transformations
  Transform transform;
  auto* child = object->FirstChildElement();
  while (child) {
    const auto& item = std::string(child->Value());
    if (item == "position") {
      transform.translation = parseVectorItem(child);
    } else if (item == "rotation") {
      Vector rotationsByAxis = parseVectorItem(child);
      transform.rotation = Matrix::fromRotation(Vector(1, 0, 0), rotationsByAxis[0])
                         * Matrix::fromRotation(Vector(0, 1, 0), rotationsByAxis[1])
                         * Matrix::fromRotation(Vector(0, 0, 1), rotationsByAxis[2]);
    } else if (item == "scale") {
      transform.scale = parseVectorItem(child);
    } else {
      std::cout << "Unknown object field: " << item << std::endl;
      return false;
    }

    child = child->NextSiblingElement();
  }

  // set the material
  std::string materialName = parseStringAttribute(object, "material");
  if (!materialName.empty()) {
    objectMaterialMap[objectList.size()] = materialName;
  }
  obj->setTransform(transform);
  objectList.push_back(obj);

  return true;
}

bool XMLParser::initializeLight(tinyxml2::XMLElement* const light) {
  std::shared_ptr<Light> l;
  auto intensity = parseFloatAttribute(light, "intensity");
  if (intensity == 0) {
    intensity = 1;
  }

  // load the color of the light
  auto* colorNode = light->FirstChildElement("color");
  Color color(1.0, 1.0, 1.0);
  if (colorNode) {
    color = Color(parseVectorItem(colorNode, "r", "g", "b"));
  }

  // create light based on type
  const auto type = parseStringAttribute(light, "type");
  if (type == "point") {
    l = std::make_shared<PointLight>(color, intensity);
  } else if (type == "sphere") {
    l = std::make_shared<SphereLight>(color, intensity, parseFloatAttribute(light, "radius"));
  } else {
    PARSE_ERROR("Unknown light type: " << type);
    return false;
  }

  // parse light transformations
  Transform transform;
  auto* child = light->FirstChildElement();
  while (child) {
    const auto& item = std::string(child->Value());
    if (item == "color") {
      child = child->NextSiblingElement();
      continue;
    }

    if (item == "position") {
      transform.translation = parseVectorItem(child);
    } else if (item == "rotation") {
      Vector rotationsByAxis = parseVectorItem(child);
      transform.rotation = Matrix::fromRotation(Vector(1, 0, 0), rotationsByAxis[0])
                         * Matrix::fromRotation(Vector(0, 1, 0), rotationsByAxis[1])
                         * Matrix::fromRotation(Vector(0, 0, 1), rotationsByAxis[2]);
    } else {
      PARSE_ERROR("Unknown light field: " << item);
      return false;
    }

    child = child->NextSiblingElement();
  }
  l->setTransform(transform);
  tracer->addLight(l);

  return true;
}

bool XMLParser::initializeMaterial(tinyxml2::XMLElement* const material) {
  const auto name = parseStringAttribute(material, "name");

  auto* child = material->FirstChildElement();
  while (child) {
    const auto& item = std::string(child->Value());
    if (item == "diffuse") {
      materialMap[name] = std::make_shared<SolidMaterial>(
        name, Color(parseVectorItem(child, "r", "g", "b")));
    } else {
      PARSE_ERROR("Unknown material field: " << item);
    }
    child = child->NextSiblingElement();
  }
  return true;
}
