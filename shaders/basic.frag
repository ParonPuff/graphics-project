#version 330 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

uniform bool useTexture;
uniform sampler2D tex;

out vec4 fragColor;

void main() {
  vec3 N = normalize(fragNormal);
  vec3 L = normalize(lightPos - fragPos);
  vec3 V = normalize(viewPos - fragPos);
  vec3 R = reflect(-L, N);

  // Ambient
  vec3 ambient = materialAmbient * lightColor;

  // Diffuse
  float diff = max(dot(N, L), 0.0);
  vec3 diffuse = diff * materialDiffuse * lightColor;

  // Specular (ONLY if diff > 0)
  vec3 specular = vec3(0.0);
  if (diff > 0.0) {
      float spec = pow(max(dot(R, V), 0.0), materialShininess);
      specular = spec * materialSpecular * lightColor;
  }

  // Texture
  vec3 baseColor = materialDiffuse;
  if (useTexture)
      baseColor *= texture(tex, fragTexCoord).rgb;

  vec3 color = ambient + diffuse + specular;
  fragColor = vec4(color * baseColor, 1.0);
}
