#version 330 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

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

void main()
{
    // --- Normalize inputs ---
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(lightPos - fragPos);
    vec3 V = normalize(viewPos - fragPos);

    // --- Blinn-Phong half vector ---
    vec3 H = normalize(L + V);

    // --- Ambient ---
    vec3 ambient = materialAmbient * lightColor;

    // --- Diffuse (Lambert) ---
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * materialDiffuse * lightColor;

    // --- Specular (Blinn-Phong) ---
    float specAngle = max(dot(N, H), 0.0);
    float spec = pow(specAngle, materialShininess);
    vec3 specular = spec * materialSpecular * lightColor;

    // --- Texture (optional albedo modulation) ---
    vec3 albedo = materialDiffuse;
    if (useTexture)
        albedo *= texture(tex, fragTexCoord).rgb;

    // --- Final lighting ---
    vec3 color = ambient + diffuse + specular;

    fragColor = vec4(color * albedo, 1.0);
}
