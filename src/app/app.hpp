#pragma once
#include "../core/renderer.hpp"
#include "../core/window.hpp"
#include "core/camera.hpp"
#include "core/input.hpp"
#include "vec3.hpp"
#include <obj_loader.hpp>
#include <types.hpp>

class App : public InputHandler {
public:
  App();
  void run();
  void loadObject(const std::string &filename);

  void onKey(int key, int scancode, int action, int mods) override;
  void onMouseMove(double xpos, double ypos) override;

private:
  Window m_window;
  Renderer m_renderer;
  Camera camera;
  matvec::mat4 m_modelMatrix;
  float farZ, fov, top, a, phi;

  bool m_mouseRotating = false;
  bool useTexture = false;

  matvec::vec3 lightPos = matvec::vec3();
  matvec::vec3 lightColor = matvec::vec3(1.0f, 1.0f, 1.0f);

  matvec::vec3 materialAmbient = {0.1f, 0.1f, 0.1f};
  matvec::vec3 materialDiffuse = {0.8f, 0.8f, 0.8f};
  matvec::vec3 materialSpecular = {1.0f, 1.0f, 1.0f};
  float shininess = 32.0f;

  double m_lastMouseX = 0.0;
  double m_lastMouseY = 0.0;
  bool m_firstMouse = true;

  void drawGui();
  void updateProjection(int mode);
};
