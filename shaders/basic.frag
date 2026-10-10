#version 330 core

in vec3 fragPos;
in vec3 fragNormal;
in vec3 fragSpherePoint;

out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

uniform bool useTexture;
uniform sampler2D tex;
#define PI 3.1415926538
void main()
{
    // --- Normalize inputs ---
    vec3 surfaceNormal = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);

    // --- Blinn-Phong half vector ---
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // --- Ambient ---
    vec3 ambient = materialAmbient * lightColor;

    // --- Diffuse (Lambert) ---
    float diff = max(dot(surfaceNormal, lightDir), 0.0);
    vec3 diffuse = diff * (materialDiffuse * lightColor);

    // --- Specular (Blinn-Phong) ---
    float specAngle = max(dot(surfaceNormal, halfwayDir), 0.0);
    float spec = pow(specAngle, materialShininess);

    // smooth fade near the terminator instead of a hard if
    spec *= smoothstep(0.0, 0.25, diff);

    // --- Texture (optional albedo modulation) ---
    vec3 albedo = materialDiffuse;

    if (useTexture){
	
      //float s = std::atan2(pointOnSphere.z, pointOnSphere.x) / (2.f * PI) + 0.5f; // longitude (u)
      //float t = std::acos(std::clamp(pointOnSphere.y / radius, -1.f, 1.f)) / M_PI; // latitude (v)
      float radius = length(fragSpherePoint);
      float s = atan(fragSpherePoint.z, fragSpherePoint.x) / (2.f * PI) + 0.5f;
      float t = acos(fragSpherePoint.y / radius) / PI;

      vec2 texCoord = vec2(s,t);
      albedo *= texture(tex, texCoord).rgb;
    }

    // --- Final lighting ---
    vec3 specular = spec * materialSpecular * lightColor;
    vec3 color = ambient + diffuse + specular;

    ambient  = materialAmbient * albedo * lightColor;
    diffuse  = diff * albedo * lightColor;

    fragColor = vec4(ambient + diffuse + specular, 1.0);   
    //fragColor = vec4(color * albedo, 1.0);
    //fragColor = vec4(normalize(fragNormal) * 0.5 + 0.5, 1.0);
    //fragColor = vec4(diffuse,1.0);
    //fragColor = vec4(vec3(diff), 1.0);
}
