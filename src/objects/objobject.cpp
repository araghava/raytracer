#include "objobject.h"

#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "../core/tiny_obj_loader.h"

bool ObjObject::load(const std::string& fileName) {
  std::cout << "Loading " << fileName << std::endl;

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string err;
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(),
                              "../models/", /*triangulate=*/true);
  if (!err.empty()) {
    std::cerr << err << std::endl;
  }

  if (!ret) {
    printf("Failed to load/parse .obj.\n");
    return false;
  }

  return true;
}

Color ObjObject::sampleTexture(const Vector& pt) const {
  return Color(0, 0, 0);
}
