#include "window.hpp"
#include "core/input.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

Window::Window(int width, int height, const char *title) {
  if (!glfwInit())
    throw std::runtime_error("Failed to initialize glfw");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!m_window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwSetWindowUserPointer(m_window, nullptr);

  glfwSetKeyCallback(m_window, [](GLFWwindow *win, int key, int scancode, int action, int mods) {
    auto *handler = static_cast<InputHandler *>(glfwGetWindowUserPointer(win));
    if (handler)
      handler->onKey(key, scancode, action, mods);
  });

  glfwSetMouseButtonCallback(m_window, [](GLFWwindow *win, int button, int action, int mods) {
    auto *handler = static_cast<InputHandler *>(glfwGetWindowUserPointer(win));
    if (handler)
      handler->onMouseButton(button, action, mods);
  });

  glfwSetCursorPosCallback(m_window, [](GLFWwindow *win, double xpos, double ypos) {
    auto *handler = static_cast<InputHandler *>(glfwGetWindowUserPointer(win));
    if (handler)
      handler->onMouseMove(xpos, ypos);
  });
//  glfwSetWindowSizeCallback(m_window,[](GLFWwindow *win, int width, int height){
//    glViewport(0,0,width,height);
//  });

//  glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *win, int w, int h) {
 //   glViewport(0, 0, w, h);
  //});

  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1);

  static bool loaded = false;
  if (!loaded) {
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
      throw std::runtime_error("glew init error: " + std::string(reinterpret_cast<const char *>(glewGetErrorString(glewError))));
    }
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
  glEnable(GL_DEPTH_TEST);
}

Window::~Window() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(m_window);
  glfwTerminate();
}

bool Window::shouldClose() const { return glfwWindowShouldClose(m_window); }

void Window::pollEvents() const { glfwPollEvents(); }

void Window::swapBuffers() const { glfwSwapBuffers(m_window); }

void Window::setInputHandler(InputHandler *handler) {
  glfwSetWindowUserPointer(m_window, handler);
}
