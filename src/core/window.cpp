#include "window.hpp"
#include "core/input.hpp"
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

  glfwSetWindowUserPointer(m_window, nullptr);
  glfwSetKeyCallback(m_window, [](GLFWwindow* win, int key, int scancode, int action, int mods){
      auto* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(win));
      if (handler)
          handler->onKey(key, scancode, action, mods);
  });

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

void Window::setInputHandler(InputHandler* handler) {
  glfwSetWindowUserPointer(m_window,handler);
}
