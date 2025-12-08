#pragma once
#include <types.hpp>

enum class ProjectionTyoe {
  PERSPECTIVE,
  ORTHOGRAPHIC,
  OBLIQUE
};

struct projectionSettings {
  ProjectionTyoe type;
  float top;
  float farZ;
  float fov;
  float a;
  float phi;
};

matvec::mat4 makePerspective(float fovDeg, float aspect, float nearZ, float farZ);
matvec::mat4 makeOrthographic(float left, float right, float bottom, float top, float nearZ,float farZ);
matvec::mat4 makeOblique(matvec::mat4 ortho, float a, float phiDeg);
