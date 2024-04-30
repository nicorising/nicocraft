#version 460 core

in vec3 fragPos;
in vec3 normal;
in vec3 color;

out vec4 FragColor;

uniform vec3 sunDir;
uniform vec3 sunColor;

void main() {
    float ambientStrength = 0.4f;
    vec3 ambient = ambientStrength * sunColor;
  	
    float sunDiff = max(dot(normal, normalize(-sunDir)), 0.0f);
    vec3 sunDiffuse = sunDiff * sunColor;

    float skyDiff = max(dot(normal, vec3(0.0f, 0.1f, 0.0f)), 0.0f);
    vec3 skyDiffuse = skyDiff * sunColor;
            
    FragColor = vec4((ambient + sunDiffuse + skyDiffuse) * color, 1.0f);
}
