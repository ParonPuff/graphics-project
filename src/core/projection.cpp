#include "projection.hpp"

matvec::mat4 makePerspective(float fovDeg, float aspect, float nearZ, float farZ){
  float fov = fovDeg * (M_PI / 180.0f);

  float t = nearZ * std::tan(fov * 0.5f);
  float b = -t;
  float r = t * aspect;
  float l = -r;

  return matvec::mat4(
      (2*nearZ)/(r-l),     0,                  (r+l)/(r-l),                 0,
      0,                   (2*nearZ)/(t-b),    (t+b)/(t-b),                 0,
      0,                   0,                 -(farZ+nearZ)/(farZ-nearZ),  (-2*farZ*nearZ)/(farZ-nearZ),
      0,                   0,                 -1,                            0
  );
}

matvec::mat4 makeOrthographic(float left, float right, float bottom, float top, float nearZ,float farZ){
  return matvec::mat4(
        2/(right-left),0,             0,              -(right+left)/(right-left),
        0,             2/(top-bottom),0,              -(top+bottom)/(top-bottom),
        0,             0,             -2/(farZ-nearZ),-(farZ+nearZ)/(farZ-nearZ),
        0,             0,             0,              1
  );
}

matvec::mat4 makeOblique(matvec::mat4 ortho, float a, float phiDeg) {
  float phi = phiDeg * (M_PI / 180.0f);
  float L = a * std::cos(phi);
  float K = a * std::sin(phi);

  return matvec::mat4(
    1, 0, L, 0,
    0, 1, K, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  ) * ortho;
}
