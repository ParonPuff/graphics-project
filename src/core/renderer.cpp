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

  unsigned int indices[] = {0, 1, 2};
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  indexCount = 3;

  glBindVertexArray(0);

  shader = new Shader("shaders/basic.vert", "shaders/basic.frag");
}

Renderer::~Renderer() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  delete shader;
}

void Renderer::loadMesh(const Mesh &mesh) {
  if (VBO)
    glDeleteBuffers(1, &VBO);
  if (EBO)
    glDeleteBuffers(1, &EBO);
  if (VAO)
    glDeleteVertexArrays(1, &VAO);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  std::vector<float> vertexData;
  vertexData.reserve(mesh.positions.size() * mesh.normals.size() +
                     mesh.uv.size());

  for (size_t i = 0; i < mesh.positions.size(); ++i) {
    vertexData.push_back(mesh.positions[i].x);
    vertexData.push_back(mesh.positions[i].y);
    vertexData.push_back(mesh.positions[i].z);

    vertexData.push_back(mesh.normals[i].x);
    vertexData.push_back(mesh.normals[i].y);
    vertexData.push_back(mesh.normals[i].z);

    vertexData.push_back(mesh.uv[i].x);
    vertexData.push_back(mesh.uv[i].y);
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

  const GLsizei stride = 8 * sizeof(float);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  std::cout << "Loaded mesh: " << mesh.positions.size() << " vertices, "
            << indexCount << " indices" << std::endl;
}
void Renderer::setModelMatrix(const matvec::mat4 &m) { m_matrix = m; }

void Renderer::setViewMatrix(const matvec::mat4 &v) { v_matrix = v; }

void Renderer::setProjectionMatrix(const matvec::mat4 &p) { p_matrix = p; }

void Renderer::draw() {
  if (!shader || VAO == 0) {
    return; // Not initialized yet
  }

  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader->use();
  // vert
  glUniformMatrix4fv(glGetUniformLocation(shader->id(), "M"), 1, GL_TRUE,
                     m_matrix.data);
  glUniformMatrix4fv(glGetUniformLocation(shader->id(), "V"), 1, GL_TRUE,
                     v_matrix.data);
  glUniformMatrix4fv(glGetUniformLocation(shader->id(), "P"), 1, GL_TRUE,
                     p_matrix.data);

  // frag
  //  uniform vec3 lightPos;
  //  uniform vec3 lightColor;
  //  uniform vec3 viewPos;
  //
  //  uniform vec3 materialAmbient;
  //  uniform vec3 materialDiffuse;
  //  uniform vec3 materialSpecular;
  //  uniform float materialShininess;
  //
  //  uniform bool useTexture;
  //  uniform sampler2D tex;
  // glUniformMatrix4fv(glGetUniformLocation(shader->id(), "lightPos"), 1,
  // GL_TRUE,
  //                    nullptr);
  // glUniformMatrix4fv(glGetUniformLocation(shader->id(), "lightColor"), 1,
  // GL_TRUE,
  //                    nullptr);
  // glUniformMatrix4fv(glGetUniformLocation(shader->id(), "viewPos"), 1,
  // GL_TRUE,
  //                    nullptr);

  glBindVertexArray(VAO);

  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}
