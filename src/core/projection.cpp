#include "projection.hpp"

matvec::mat4 makePerspective(float fovDeg, float aspect, float nearZ,
                             float farZ) {
  float fovRad = fovDeg * (static_cast<float>(M_PI) / 180.0f);
  float f = 1.0f / std::tan(fovRad * 0.5f);

  return matvec::mat4(
      f / aspect, 0, 0,                                  0,
      0,          f, 0,                                  0,
      0,          0, -(farZ + nearZ) / (farZ - nearZ),  -2 * farZ * nearZ / (farZ - nearZ),
      0,          0, -1,                                 0
  );
}

matvec::mat4 makeOrthographic(float left, float right, float bottom, float top,
                              float nearZ, float farZ) {
  return matvec::mat4(
      2 / (right - left), 0, 0, -(right + left) / (right - left), 0,
      2 / (top - bottom), 0, -(top + bottom) / (top - bottom), 0, 0,
      -2 / (farZ - nearZ), -(farZ + nearZ) / (farZ - nearZ), 0, 0, 0, 1);
}

matvec::mat4 makeOblique(matvec::mat4 ortho, float a, float phiDeg) {
  float phi = phiDeg * (M_PI / 180.0f);
  float L = a * std::cos(phi);
  float K = a * std::sin(phi);

  return matvec::mat4(1, 0, L, 0, 0, 1, K, 0, 0, 0, 1, 0, 0, 0, 0, 1) * ortho;
}
