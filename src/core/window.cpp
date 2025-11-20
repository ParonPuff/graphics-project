#include "window.hpp"
#include <stdexcept>

Window::Window(int width, int height, const char* title) {
  if(!glfwInit())
      throw std::runtime_error("Failed to initialize glfw");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window = glfwCreateWindow(width,height,title,nullptr,nullptr);
  if(!m_window){
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(m_window);
}

Window::~Window() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

bool Window::shouldClose() const {
  return glfwWindowShouldClose(m_window);
}

void Window::pollEvents() const {
  glfwPollEvents();
}

void Window::swapBuffers() const {
  glfwSwapBuffers(m_window);
}
