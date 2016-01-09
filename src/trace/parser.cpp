#include "parser.h"

#include "../core/vector3.h"
#include "../core/color.h"
#include "../objects/object.h"
#include "../objects/sphere.h"
#include "../objects/plane.h"

#define MAX_PARSER_LINE_COUNT 10000000

Parser::Parser() {
  // TODO: std::make_shared
  textureMap["null"] = std::shared_ptr<Texture>(new NoopTexture("null"));
}

bool Parser::createWorld(Raytracer *tracer, std::string &err, std::istream &is) {
  int count = 0;

  while (1) {
    std::string tag;
    is >> tag;
    if (tag == "lock") {
      return true;
    } else if (tag == "parameters") {
      int pc;
      is >> pc;
      parseParameters(tracer, pc, is);
    } else if (tag == "objects") {
      int oc;
      is >> oc;
      parseObjects(tracer, oc, is);
    } else if (tag == "lights") {
      int lc;
      is >> lc;
      parseLights(tracer, lc, is);
    } else if (tag == "textures") {
      int tc;
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

bool Parser::parseTextures(Raytracer *tracer, int tc, std::istream &is) {
  for (int i = 0; i < tc; i++) {
    std::string name, tag;
    is >> name >> tag;
    if (tag == "solid") {
      double r, g, b;
      is >> r >> g >> b;

      // TODO: upgrade compiler and use make_shared
      textureMap[name] = std::shared_ptr<SolidTexture>(new SolidTexture(name, Color(r, g, b)));
    }
  }

  return true;
}

bool Parser::parseParameters(Raytracer *tracer, int pc, std::istream &is) {
  RenderParms parms;

  for (int i = 0; i < pc; i++) {
    std::string tag;
    is >> tag;
    if (tag == "resolution") {
      int rx, ry;
      is >> rx >> ry;
      parms.width = rx;
      parms.height = ry;
    } else if (tag == "antialias") {
      int as;
      is >> as;
      parms.antialias = as;
    }
  }

  tracer->setParms(parms);
  return true;
}

bool Parser::parseObjects(Raytracer *tracer, int oc, std::istream &is) {
  for (int i = 0; i < oc; i++) {
    std::string otype;
    is >> otype;

    Object *o;
    std::string tex;
    is >> tex;
    is >> tex;

    if (otype == "sphere") {
      std::string tmp;
      is >> tmp;
      float x, y, z, rad;
      is >> x >> y >> z;
      is >> tmp >> rad;
      o = new Sphere(Vector(x, y, z), rad);
    } else if (otype == "plane") {
      float x1, y1, z1, x2, y2, z2, x3, y3, z3;
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
      o = new Plane(v1, v2, v3);
    }

    o->setTexture(textureMap[tex]);
    tracer->addObject(o);
  }
  return true;
}

bool Parser::parseLights(Raytracer *tracer, int lc, std::istream &is) {
  for (int i = 0; i < lc; i++) {
    std::string ltype;
    is >> ltype;
    if (ltype == "spherelight") {
      std::string tmp;
      is >> tmp;
      float x, y, z;
      is >> x >> y >> z;
      is >> tmp;
      float r, g, b;
      is >> r >> g >> b;
      is >> tmp;
      float inten, rad;
      is >> inten;
      is >> tmp;
      is >> rad;

      tracer->addLight(
          new SphereLight(Vector(x, y, z), Color(r, g, b), inten, rad));
    } else if (ltype == "pointlight") {
      std::string tmp;
      is >> tmp;
      float x, y, z;
      is >> x >> y >> z;
      is >> tmp;
      float r, g, b;
      is >> r >> g >> b;
      is >> tmp;
      float inten;
      is >> inten;

      tracer->addLight(new PointLight(Vector(x, y, z), Color(r, g, b), inten));
    }
  }
  return true;
}
