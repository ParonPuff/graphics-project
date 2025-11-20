#version 330 core

layout(location = 0) in vec3 inPos;

uniform mat4 uModel;
void main() {
  gl_Position = uModel * vec4(inPos, 1.0);
}
