#pragma once

#include "shader.hpp"
#include <GL/glew.h>
#include <obj_loader.hpp>
#include <types.hpp>

class Renderer {
public:
  Renderer();
  ~Renderer();

  void draw();
  // void setWireframe(bool enabled);
  void setModelMatrix(const matvec::mat4 &M);
  void loadMesh(const Mesh &m);

private:
  GLuint VAO = 0, VBO = 0, EBO = 0;
  size_t indexCount = 0;
  matvec::mat4 m_matrix;
  Shader *shader;
};
