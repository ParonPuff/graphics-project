#include "camera.hpp"

Camera::Camera() {
  p0 = matvec::vec3(0, 0, 2);
  pref = matvec::vec3(0, 0, 0);
  vup = matvec::vec3(0, 1, 0);
  dist = 2.0f;

  updateViewMatrix();
}

void Camera::updateViewMatrix() {
  matvec::vec3 f = (pref - p0).normalized();
  matvec::vec3 s = f.cross(vup).normalized();
  matvec::vec3 u = s.cross(f);

  V = matvec::mat4(s.x, s.y, s.z, -s.dot(p0), u.x, u.y, u.z, -u.dot(p0), -f.x,
                   -f.y, -f.z, f.dot(p0), 0, 0, 0, 1);
  Vinv = matvec::mat4(s.x, u.x, -f.x, p0.x, s.y, u.y, -f.y, p0.y, s.z, u.z,
                      -f.z, p0.z, 0, 0, 0, 1);
}

const matvec::mat4 &Camera::getViewMatrix() { return V; }
const matvec::mat4 &Camera::getInverseViewMatrix() { return Vinv; }

void Camera::moveLocal(float dx, float dy, float dz) {
  matvec::vec3 right(Vinv.data[0], Vinv.data[4], Vinv.data[8]);
  matvec::vec3 up(Vinv.data[1], Vinv.data[5], Vinv.data[9]);
  matvec::vec3 forward(Vinv.data[2], Vinv.data[6], Vinv.data[10]);

  // Compute movement in world space
  matvec::vec3 delta = right * dx + up * dy + forward * dz;

  // Move both camera position and reference point
  p0 = p0 + delta;
  pref = pref + delta;

  updateViewMatrix();
}

void Camera::rotateYaw(float r) {
  // Get current direction
  matvec::vec3 dir = (pref - p0).normalized();

  // Rotate direction around world Y-axis
  matvec::mat4 R = Vinv * matvec::mat4::rotateY(r) * V;
  matvec::vec4 newDir = R * matvec::vec4(dir, 0.0f);

  // Update reference point at fixed distance
  pref = p0 + matvec::vec3(newDir) * dist;

  updateViewMatrix();
}

void Camera::rotatePitch(float r) {
  matvec::vec3 dir = pref - p0;

  matvec::mat4 R = Vinv * matvec::mat4::rotateX(r) * V;
  matvec::vec4 newDir = R * matvec::vec4(dir, 0.0f);

  pref = p0 + matvec::vec3(newDir);
  updateViewMatrix();
}
