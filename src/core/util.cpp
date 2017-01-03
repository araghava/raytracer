#include "util.h"
#include <cstdlib>
#include "geometry.h"

float UTILrandomFloatBetween(float min, float max) {
  const float r = ((float)rand()) / (float)RAND_MAX;
  return (r * (max - min)) + min;
}

Vector UTILreflectVector(const Vector& incident, const Vector& nml) {
  const float perp = 2.0 * incident.dot(nml);
  return (incident - (nml * perp)).normalize();
}

Vector UTILinterpolateFace(const std::vector<Vector>& values, const Vector& bc) {
  return (values[0] * bc.x) + (values[1] * bc.y) + (values[2] * bc.z);
}

Vector UTILgetFaceVertex(
  const std::array<tinyobj::index_t, 3>& face, const tinyobj::attrib_t& attrib, size_t v) {
  return Vector(
    attrib.vertices[3 * face[v].vertex_index + 0],
    attrib.vertices[3 * face[v].vertex_index + 1],
    attrib.vertices[3 * face[v].vertex_index + 2]);
}

Vector UTILgetFaceNormal(
  const std::array<tinyobj::index_t, 3>& face, const tinyobj::attrib_t& attrib, size_t v) {
  return Vector(
    attrib.normals[3 * face[v].normal_index + 0],
    attrib.normals[3 * face[v].normal_index + 1],
    attrib.normals[3 * face[v].normal_index + 2]);
}

Vector UTILgetFaceTexCoord(
  const std::array<tinyobj::index_t, 3>& face, const tinyobj::attrib_t& attrib, size_t v) {
  return Vector(
    attrib.texcoords[2 * face[v].texcoord_index + 0],
    attrib.texcoords[2 * face[v].texcoord_index + 1], 0);
}

Vector UTILtransformVector(
  const Vector& vector, const Transform& transform) {
  return transform.rotation * (vector * transform.scale) + transform.translation;
}
