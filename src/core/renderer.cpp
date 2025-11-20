#include "renderer.hpp"
#include "core/shader.hpp"
#include <GL/gl.h>
#include <stdexcept>
#include <vector>

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

void Renderer::loadMesh(const Mesh &mesh) {
  if (VAO) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  std::vector<float> vertexData;
  vertexData.reserve(mesh.positions.size() * 3);

  for (auto &v : mesh.positions) {
    vertexData.push_back(v.x);
    vertexData.push_back(v.y);
    vertexData.push_back(v.z);
  }

  indexCount = mesh.indices.size();

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
               vertexData.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned),
               mesh.indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}
void Renderer::setModelMatrix(const matvec::mat4 &m) { m_matrix = m; }

void Renderer::draw() {
  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader->use();

  GLint loc = glGetUniformLocation(shader->id(), "uModel");
  glUniformMatrix4fv(loc, 1, GL_TRUE, m_matrix.data);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}
