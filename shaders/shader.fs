#version 460 core

out vec4 fragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2) *
                vec4(color, 1.0);
}
