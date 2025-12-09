#pragma once
#include "../core/renderer.hpp"
#include "../core/window.hpp"
#include "core/camera.hpp"
#include "core/input.hpp"
#include <obj_loader.hpp>
#include <types.hpp>

class App : public InputHandler {
public:
  App();
  void run();
  void loadObject(const std::string &filename);

  void onKey(int key, int scancode, int action, int mods) override;
  void onMouseButton(int button, int action, int mods) override;
  void onMouseMove(double xpos, double ypos) override;

private:
  Window m_window;
  Renderer m_renderer;
  Camera camera;
  matvec::mat4 m_modelMatrix;
  float farZ, fov, top, a, phi;

  bool m_mouseRotating = false;
  double m_lastMouseX = 0.0;
  double m_lastMouseY = 0.0;
  bool m_firstMouse = true;

  void drawGui();
  void updateProjection(int mode);
};
