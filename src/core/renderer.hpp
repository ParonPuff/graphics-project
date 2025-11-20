#pragma once

#include "shader.hpp"
#include <GL/glew.h>
#include <types.hpp>

class Renderer {
public:
  Renderer();
  ~Renderer();

  // void loadMesh(const Mesh& m);
  void setModelMatrix(const matvec::mat4 &M);
  void draw();
  // void setWireframe(bool enabled);

private:
  GLuint VAO, VBO;
  Shader *shader;
  matvec::mat4 model;
};
