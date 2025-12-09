#pragma once
#include <GLFW/glfw3.h>

class InputHandler {
public:
  virtual ~InputHandler() = default;
  virtual void onKey(int key, int scancode, int action, int mods) = 0;
  virtual void onMouseButton(int button, int action, int mods) {};
  virtual void onMouseMove(double xpos, double ypos) {};
};
