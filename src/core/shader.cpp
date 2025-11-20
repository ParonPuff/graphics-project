#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string Shader::loadFile(const std::string &path) {
  std::ifstream f(path);
  if (!f.is_open())
    throw std::runtime_error("Cannot open shader file: " + path);

  std::stringstream ss;
  ss << f.rdbuf();
  return ss.str();
}

GLuint Shader::compile(GLenum type, const std::string &src) {
  GLuint shader = glCreateShader(type);
  const char *cstr = src.c_str();
  glShaderSource(shader, 1, &cstr, nullptr);
  glCompileShader(shader);

  GLint ok;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    char log[512];
    glGetShaderInfoLog(shader, 512, nullptr, log);
    throw std::runtime_error("shader compile error: " + std::string(log));
  }

  return shader;
}

Shader::Shader(const std::string &vert, const std::string &frag) {
  std::string vsrc = loadFile(vert);
  std::string fsrc = loadFile(frag);

  GLuint vs = compile(GL_VERTEX_SHADER, vsrc);
  GLuint fs = compile(GL_FRAGMENT_SHADER, fsrc);

  m_program = glCreateProgram();
  glAttachShader(m_program, vs);
  glAttachShader(m_program, fs);

  glLinkProgram(m_program);

  GLint ok;
  glGetProgramiv(m_program, GL_LINK_STATUS, &ok);

  if (!ok) {
    char log[512];
    glGetProgramInfoLog(m_program, 512, nullptr, log);
    throw std::runtime_error("Shader link error: " + std::string(log));
  }

  glDeleteShader(vs);
  glDeleteShader(fs);
}

Shader::~Shader() { glDeleteProgram(m_program); }

void Shader::use() const { glUseProgram(m_program); }
