#pragma once
#include <GLFW/glfw3.h>

class InputHandler {
  public:
    virtual void onKey(int key, int scancode, int action, int mods) = 0;
    virtual ~InputHandler() = default;
};
