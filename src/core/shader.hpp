#pragma once
#include <GL/glew.h>
#include <string>

class Shader {
public:
  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  ~Shader();

  void use() const;
  GLuint id() const { return m_program; };

private:
  GLuint m_program;
  std::string loadFile(const std::string &path);
  GLuint compile(GLenum type, const std::string &src);
};
