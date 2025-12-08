
#pragma once
#include <types.hpp>

class Camera {
public:
  Camera();

  void rotateYaw(float radians);
  void rotatePitch(float radians);

  void moveLocal(float dx, float dy, float dz);

  const matvec::mat4 &getViewMatrix();
  const matvec::mat4 &getInverseViewMatrix();

private:
  matvec::vec3 p0;
  matvec::vec3 pref;
  matvec::vec3 vup;

  float dist;
  matvec::mat4 V;
  matvec::mat4 Vinv;

  void updateViewMatrix();
};
