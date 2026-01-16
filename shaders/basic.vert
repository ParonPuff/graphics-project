#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main()
{
    vec4 worldPos = M * vec4(vPosition, 1.0);
    fragPos = worldPos.xyz;

    fragNormal = mat3(transpose(inverse(M))) * vNormal;
    fragTexCoord = vTexCoord;

    gl_Position = P * V * worldPos;
}
