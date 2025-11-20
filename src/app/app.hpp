#pragma once
#include "../core/renderer.hpp"
#include "../core/window.hpp"
#include <types.hpp>

class App {
public:
  App();
  void run();

private:
  Window m_window;
  Renderer m_renderer;
  matvec::mat4 m_modelMatrix;
};
