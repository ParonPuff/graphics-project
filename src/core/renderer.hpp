#pragma once

#include "shader.hpp"
#include "vec3.hpp"
#include <obj_loader.hpp>
#include <types.hpp>

class Renderer {
public:
  Renderer();
  ~Renderer();

  void draw();
  void setModelMatrix(const matvec::mat4 &m);
  void setViewMatrix(const matvec::mat4 &v);
  void setProjectionMatrix(const matvec::mat4 &p);

  void setLightPos(const matvec::vec3 &lightP);
  void setLightColor(const matvec::vec3 &lightC);
  void setViewPos(const matvec::vec3 &viewP);

  void setAmbient(const matvec::vec3 &ambient);
  void setDiffuse(const matvec::vec3 &diffuse);
  void setSpecular(const matvec::vec3 &specular);

  void setShininess(const float &shiny);

  void setUseTexture(const bool &useTexture);

  void loadMesh(const Mesh &m);
  void loadTexture(const std::string &path);

private:
  GLuint VAO = 0, VBO = 0, EBO = 0;
  size_t indexCount = 0;

  matvec::mat4 m_matrix;
  matvec::mat4 v_matrix;
  matvec::mat4 p_matrix;

  matvec::vec3 lightPos;
  matvec::vec3 lightColor;
  matvec::vec3 viewPos;

  matvec::vec3 materialAmbient;
  matvec::vec3 materialDiffuse;
  matvec::vec3 materialSpecular;

  float materialShininess;

  GLuint textureID = 0;
  bool useTexture, textureLoaded = false;

  Shader *shader;
};
