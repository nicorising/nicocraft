#version 460 core

in vec3 fragPos;
in vec3 normal;
in vec3 color;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main() {
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
  	
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;
            
    FragColor = vec4((ambient + diffuse) * color, 1.0f);
}
