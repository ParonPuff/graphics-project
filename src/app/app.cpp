#include "app.hpp"
#include "mat4.hpp"
#include <iostream>

App::App() : m_window(800, 600, "Studio 3D") {
  m_modelMatrix = matvec::mat4::identity();
  m_window.setInputHandler(this);
}

void App::loadObject(const std::string &filename) {
  ObjLoader loader;
  Mesh mesh = loader.load(filename);

  m_modelMatrix = matvec::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

  m_renderer.loadMesh(mesh);
}

void App::run() {
  while (!m_window.shouldClose()) {
    m_window.pollEvents();

    m_renderer.setModelMatrix(m_modelMatrix);
    m_renderer.draw();

    m_window.swapBuffers();
  }
}
void App::onKey(int key, int scancode, int action, int mods) {
  if (action != GLFW_PRESS && action != GLFW_REPEAT)
    return;

  for (auto e : m_modelMatrix.data) {
    std::cout << e;
  }
  std::cout << '\n';
  switch (key) {
  case GLFW_KEY_J:
    m_modelMatrix = m_modelMatrix * matvec::mat4::translate(-0.1, 0, 0);
    break;
  case GLFW_KEY_L:
    m_modelMatrix = m_modelMatrix * matvec::mat4::translate(0.1, 0, 0);
    break;
  case GLFW_KEY_I:
    m_modelMatrix = m_modelMatrix * matvec::mat4::translate(0, 0.1, 0);
    break;
  case GLFW_KEY_K:
    m_modelMatrix = m_modelMatrix * matvec::mat4::translate(0, -0.1, 0);
    break;
  case GLFW_KEY_UP:
    m_modelMatrix = m_modelMatrix * matvec::mat4::rotateX(.17f);
    break;
  case GLFW_KEY_DOWN:
    m_modelMatrix = m_modelMatrix * matvec::mat4::rotateX(-.17f);
    break;
  case GLFW_KEY_LEFT:
    m_modelMatrix = m_modelMatrix * matvec::mat4::rotateY(.17f);
    break;
  case GLFW_KEY_RIGHT:
    m_modelMatrix = m_modelMatrix * matvec::mat4::rotateY(-.17f);
    break;
  case GLFW_KEY_O:
    std::cout << "Enter object file: ";
    std::string file;
    std::cin >> file;
    loadObject(file);
    break;
  }
}
