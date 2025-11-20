#include "app.hpp"

App::App():m_window(800,600, "Studio 3D") {
  m_modelMatrix = matvec::mat4(
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
  );
}

void App::run() {
  while (!m_window.shouldClose()) {
    m_window.pollEvents();

    m_renderer.setModelMatrix(m_modelMatrix);
    m_renderer.draw();

    m_window.swapBuffers();
  }
}
