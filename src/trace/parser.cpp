#include "parser.h"

#include <iostream>

#include "../core/vector3.h"
#include "../core/color.h"
#include "../objects/object.h"
#include "../objects/objobject.h"
#include "../objects/primitives/sphere.h"
#include "../objects/primitives/plane.h"

#define MAX_PARSER_LINE_COUNT 10000000

Parser::Parser() {
  textureMap["null"] = std::make_shared<NoopTexture>("null");
}

bool Parser::createWorld(Raytracer* const tracer, std::string &err, std::istream &is) {
  int count = 0;

  while (1) {
    int pc, oc, lc, tc;
    std::string tag; is >> tag;
    if (tag == "lock") {
      return true;
    } else if (tag == "parameters") {
      is >> pc;
      parseParameters(tracer, pc, is);
    } else if (tag == "objects") {
      is >> oc;
      parseObjects(tracer, oc, is);
    } else if (tag == "lights") {
      is >> lc;
      parseLights(tracer, lc, is);
    } else if (tag == "textures") {
      is >> tc;
      parseTextures(tracer, tc, is);
    }

    if (count == MAX_PARSER_LINE_COUNT) {
      err += "Exceeded maximum line count. Did you add a lock statement?";
      return false;
    }
    count++;
  }
}

bool Parser::parseTextures(Raytracer* const tracer, const int tc, std::istream &is) {
  std::string name, tag;
  double r, g, b;
  for (int i = 0; i < tc; i++) {
    is >> name >> tag;
    if (tag == "solid") {
      is >> r >> g >> b;
      textureMap[name] = std::make_shared<SolidTexture>(name, Color(r, g, b));
    }
  }
  return true;
}

bool Parser::parseParameters(Raytracer* const tracer, const int pc, std::istream &is) {
  RenderParms parms;
  std::string tag;
  int as, rx, ry;

  for (int i = 0; i < pc; i++) {
    is >> tag;
    if (tag == "resolution") {
      is >> rx >> ry;
      parms.width = rx;
      parms.height = ry;
    } else if (tag == "antialias") {
      is >> as;
      parms.antialias = as;
    }
  }

  tracer->setParms(parms);
  return true;
}

bool Parser::parseObjects(Raytracer* const tracer, const int oc, std::istream &is) {
  float x1, y1, z1, x2, y2, z2, x3, y3, z3;
  float x, y, z, rad;
  std::string tex, otype, tmp;
  std::shared_ptr<Object> o;

  for (int i = 0; i < oc; i++) {
    is >> otype;
    if (otype == "obj") {
      std::string filename; is >> tmp >> filename;
      is >> tmp >> x >> y >> z;
      o = std::make_shared<ObjObject>(Vector(x, y, z), filename);
    } else {
      is >> tex >> tex;
      if (otype == "sphere") {
        is >> tmp >> x >> y >> z;
        is >> tmp >> rad;
        o = std::make_shared<Sphere>(Vector(x, y, z), rad);
      } else if (otype == "plane") {
        Vector v1, v2, v3;
        std::string s;

        is >> s;
        is >> x1 >> y1 >> z1;
        v1 = Vector(x1, y1, z1);

        is >> s;
        is >> x2 >> y2 >> z2;
        v2 = Vector(x2, y2, z2);

        is >> s;
        is >> x3 >> y3 >> z3;
        v3 = Vector(x3, y3, z3);
        o = std::make_shared<Plane>(v1, v2, v3);
      }
      o->addTexture(tex, textureMap[tex]);
    }
    tracer->addObject(o);
  }

  return true;
}

bool Parser::parseLights(Raytracer* const tracer, int lc, std::istream &is) {
  float x, y, z, r, g, b, inten, rad;
  std::string tmp, ltype;
  std::shared_ptr<Light> l;

  for (int i = 0; i < lc; i++) {
    is >> ltype;
    if (ltype == "spherelight") {
      is >> tmp >> x >> y >> z;
      is >> tmp >> r >> g >> b;
      is >> tmp >> inten;
      is >> tmp >> rad;
      l = std::make_shared<SphereLight>(Vector(x, y, z), Color(r, g, b), inten, rad);
    } else if (ltype == "pointlight") {
      is >> tmp >> x >> y >> z;
      is >> tmp >> r >> g >> b;
      is >> tmp >> inten;
      l = std::make_shared<PointLight>(Vector(x, y, z), Color(r, g, b), inten);
    }
    tracer->addLight(l);
  }

  return true;
}
