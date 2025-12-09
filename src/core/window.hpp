#pragma once
#include <GL/glew.h>
#include "input.hpp"
#include <GLFW/glfw3.h>
class Window {
public:
  Window(int width, int height, const char *title);
  ~Window();

  bool shouldClose() const;
  void pollEvents() const;
  void swapBuffers() const;
  void setInputHandler(InputHandler *handler);
  GLFWwindow *raw() const { return m_window; };

private:
  GLFWwindow *m_window = nullptr;
};
