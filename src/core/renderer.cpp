#include "renderer.hpp"
#include "core/shader.hpp"
#include <GL/gl.h>
#include <stdexcept>

Renderer::Renderer() {
  static bool loaded = false;
  if (!loaded) {
    // Initialize glew
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
      throw std::runtime_error("glew init error: " +
                               std::string(reinterpret_cast<const char *>(
                                   glewGetErrorString(glewError))));
      // cout << "glew init error:" << endl << glewGetErrorString(glewError) <<
      // endl; exit(EXIT_FAILURE);
    }
  }

  // background
  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  float vertecies[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  shader = new Shader("shaders/basic.vert", "shaders/basic.frag");
}

Renderer::~Renderer() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  delete shader;
}

void Renderer::setModelMatrix(const matvec::mat4 &m) { model = m; }

void Renderer::draw() {
  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader->use();
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
