#include "mat4.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "renderer.hpp"
#include "../lib/stb_image.h"
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
  if (textureID)
    glDeleteTextures(1, &textureID);
  delete shader;
}

void Renderer::loadTexture(const std::string &path) {
  if (textureID)
    glDeleteTextures(1, &textureID);

  int width, height, channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

  if (!data) {
    std::cerr << "Failed to load texture: " << path << std::endl;
    return;
  }

  GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Texture parameters (very important)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Upload image
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);

  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  textureLoaded = true;
  return;
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
  vertexData.reserve(mesh.positions.size() * mesh.normals.size() + mesh.spherePoint.size());

  for (size_t i = 0; i < mesh.positions.size(); ++i) {
    vertexData.push_back(mesh.positions[i].x);
    vertexData.push_back(mesh.positions[i].y);
    vertexData.push_back(mesh.positions[i].z);

    vertexData.push_back(mesh.normals[i].x);
    vertexData.push_back(mesh.normals[i].y);
    vertexData.push_back(mesh.normals[i].z);

    vertexData.push_back(mesh.spherePoint[i].x);
    vertexData.push_back(mesh.spherePoint[i].y);
    vertexData.push_back(mesh.spherePoint[i].z);
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

  const GLsizei stride = 9 * sizeof(float);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  std::cout << "Loaded mesh: " << mesh.positions.size() << " vertices, "
            << indexCount << " indices" << std::endl;
}
void Renderer::setModelMatrix(const matvec::mat4 &m) { m_matrix = m; }

void Renderer::setViewMatrix(const matvec::mat4 &v) { v_matrix = v; }

void Renderer::setProjectionMatrix(const matvec::mat4 &p) { p_matrix = p; }

void Renderer::setLightPos(const matvec::vec3 &lightP) { lightPos = lightP; }

void Renderer::setLightColor(const matvec::vec3 &lightC) {
  lightColor = lightC;
}
void Renderer::setViewPos(const matvec::vec3 &viewP) { viewPos = viewP; }

void Renderer::setAmbient(const matvec::vec3 &ambient) {
  materialAmbient = ambient;
}
void Renderer::setDiffuse(const matvec::vec3 &diffuse) {
  materialDiffuse = diffuse;
}
void Renderer::setSpecular(const matvec::vec3 &specular) {
  materialSpecular = specular;
}

void Renderer::setShininess(const float &shiny) { materialShininess = shiny; }

void Renderer::setUseTexture(const bool &useTexture) {
  this->useTexture = useTexture;
}

void Renderer::draw() {
  if (!shader || VAO == 0) {
    return; // Not initialized yet
  }

  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader->use();

  // vert
  glUniformMatrix4fv(glGetUniformLocation(shader->id(), "M"), 1, GL_TRUE, m_matrix.data);
  glUniformMatrix4fv(glGetUniformLocation(shader->id(), "V"), 1, GL_TRUE, v_matrix.data);
  glUniformMatrix4fv(glGetUniformLocation(shader->id(), "P"), 1, GL_TRUE, p_matrix.data);

  // Light properties
  glUniform3f(glGetUniformLocation(shader->id(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
  glUniform3f(glGetUniformLocation(shader->id(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);

  // View position
  glUniform3f(glGetUniformLocation(shader->id(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);

  // Material properties
  glUniform3f(glGetUniformLocation(shader->id(), "materialAmbient"),  materialAmbient.x,  materialAmbient.y,  materialAmbient.z);
  glUniform3f(glGetUniformLocation(shader->id(), "materialDiffuse"),  materialDiffuse.x,  materialDiffuse.y,  materialDiffuse.z);
  glUniform3f(glGetUniformLocation(shader->id(), "materialSpecular"), materialSpecular.x, materialSpecular.y, materialSpecular.z);

  // Shininess
  glUniform1f(glGetUniformLocation(shader->id(), "materialShininess"), materialShininess);

  // Use texture flag
  glUniform1i(glGetUniformLocation(shader->id(), "useTexture"), useTexture ? 1 : 0);

  if (useTexture && textureLoaded) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shader->id(), "tex"), 0);
  }
  // frag

  glBindVertexArray(VAO);

  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

  matvec::mat4 scale = matvec::mat4::identity()*0.1;
  matvec::mat4 lightModel = scale * matvec::mat4::translate(lightPos.x,lightPos.y,lightPos.z);

  glUniformMatrix4fv(glGetUniformLocation(shader->id(), "M"), 1, GL_TRUE, lightModel.data);
  glUniformMatrix4fv(glGetUniformLocation(shader->id(), "V"), 1, GL_TRUE, v_matrix.data);
  glUniformMatrix4fv(glGetUniformLocation(shader->id(), "P"), 1, GL_TRUE, p_matrix.data);

  // Light properties
  glUniform3f(glGetUniformLocation(shader->id(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
  glUniform3f(glGetUniformLocation(shader->id(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);

  // View position
  glUniform3f(glGetUniformLocation(shader->id(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);

  // Material properties
  glUniform3f(glGetUniformLocation(shader->id(), "materialAmbient"),  materialAmbient.x,  materialAmbient.y,  materialAmbient.z);
  glUniform3f(glGetUniformLocation(shader->id(), "materialDiffuse"),  materialDiffuse.x,  materialDiffuse.y,  materialDiffuse.z);
  glUniform3f(glGetUniformLocation(shader->id(), "materialSpecular"), materialSpecular.x, materialSpecular.y, materialSpecular.z);

  // Shininess
  glUniform1f(glGetUniformLocation(shader->id(), "materialShininess"), materialShininess);

  // Use texture flag
  glUniform1i(glGetUniformLocation(shader->id(), "useTexture"), 0);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}
