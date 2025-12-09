#include "renderer.hpp"
#include "core/shader.hpp"
#include <iostream>
#include <vector>

Renderer::Renderer() {

  float vertecies[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // ADD THIS: Create indices for the triangle
  unsigned int indices[] = {0, 1, 2};
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  indexCount = 3; // ADD THIS

  glBindVertexArray(0);

  shader = new Shader("shaders/basic.vert", "shaders/basic.frag");
}

Renderer::~Renderer() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  delete shader;
}

void Renderer::loadMesh(const Mesh &mesh) {
  // if (VAO) {
  //   glDeleteVertexArrays(1, &VAO);
  //   glDeleteBuffers(1, &VBO);
  //   glDeleteBuffers(1, &EBO);
  // }
  //
  // glGenVertexArrays(1, &VAO);
  // glBindVertexArray(VAO);
  //
  // std::vector<float> vertexData;
  // vertexData.reserve(mesh.positions.size() * 3);
  //
  // for (auto &v : mesh.positions) {
  //   vertexData.push_back(v.x);
  //   vertexData.push_back(v.y);
  //   vertexData.push_back(v.z);
  // }
  //
  // indexCount = mesh.indices.size();
  //
  // glGenBuffers(1, &VBO);
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
  //              vertexData.data(), GL_STATIC_DRAW);
  //
  // glGenBuffers(1, &EBO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() *
  // sizeof(unsigned),
  //              mesh.indices.data(), GL_STATIC_DRAW);
  //
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void
  // *)0); glEnableVertexAttribArray(0); Delete old buffers
  if (VBO)
    glDeleteBuffers(1, &VBO);
  if (EBO)
    glDeleteBuffers(1, &EBO);
  if (VAO)
    glDeleteVertexArrays(1, &VAO);

  // Create new VAO
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Prepare vertex data
  std::vector<float> vertexData;
  vertexData.reserve(mesh.positions.size() * 3);

  for (auto &v : mesh.positions) {
    vertexData.push_back(v.x);
    vertexData.push_back(v.y);
    vertexData.push_back(v.z);
  }

  indexCount = mesh.indices.size();

  // Create and fill VBO
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
               vertexData.data(), GL_STATIC_DRAW);

  // Create and fill EBO
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned),
               mesh.indices.data(), GL_STATIC_DRAW);

  // Set up vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  // ADD DEBUG OUTPUT:
  std::cout << "Loaded mesh: " << mesh.positions.size() << " vertices, "
            << indexCount << " indices" << std::endl; // glBindVertexArray(0);
}
void Renderer::setModelMatrix(const matvec::mat4 &m) { m_matrix = m; }

void Renderer::setViewMatrix(const matvec::mat4 &v) { v_matrix = v; }

void Renderer::setProjectionMatrix(const matvec::mat4 &p) { p_matrix = p; }

void Renderer::draw() {
  if (!shader || VAO == 0) {
    return; // Not initialized yet
  }

  // std::cout << "entered render" << std::endl;

  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // std::cout << "shader use" << std::endl;
  shader->use();

  // std::cout << "set uniforms" << std::endl;
  glUniformMatrix4fv(glGetUniformLocation(shader->id(), "uModel"), 1, GL_TRUE,
                     m_matrix.data);
  glUniformMatrix4fv(glGetUniformLocation(shader->id(), "uView"), 1, GL_TRUE,
                     v_matrix.data);
  glUniformMatrix4fv(glGetUniformLocation(shader->id(), "uProj"), 1, GL_TRUE,
                     p_matrix.data);

  // std::cout << "bind vao" << std::endl;
  glBindVertexArray(VAO);

  // std::cout << "VAO: " << VAO << std::endl;
  // std::cout << "indexCount: " << indexCount << std::endl;
  // std::cout << "About to draw..." << std::endl;

  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

  // std::cout << "exiting render" << std::endl;
}
