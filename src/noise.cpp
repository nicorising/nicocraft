#include "glm/glm.hpp"
#include "noise.hpp"

#include <math.h>
#include <iostream>

Noise::Noise() {}

float Noise::noise(float x, float y, float scale) {
    glm::vec2 point = glm::vec2(x, y) * scale;

    glm::vec2 bottomLeft = glm::floor(point);
    glm::vec2 bottomRight = bottomLeft + glm::vec2(1.0f, 0.0f);
    glm::vec2 topLeft = bottomLeft + glm::vec2(0.0f, 1.0f);
    glm::vec2 topRight = bottomLeft + glm::vec2(1.0f, 1.0f);

    glm::vec2 diff = point - bottomLeft;

    float bottomLeftDot = glm::dot(gradient(bottomLeft), diff);
    float bottomRightDot = glm::dot(gradient(bottomRight), point - bottomRight);
    float topLeftDot = glm::dot(gradient(topLeft), point - topLeft);
    float topRightDot = glm::dot(gradient(topRight), point - topRight);

    float interpBottom = interpolate(bottomLeftDot, bottomRightDot, diff.x);
    float interpTop = interpolate(topLeftDot, topRightDot, diff.x);

    return interpolate(interpBottom, interpTop, diff.y) * 0.5f + 0.5f;
}

// Thanks, Wikipedia
glm::vec2 Noise::gradient(glm::vec2 point) {
    unsigned int a = (unsigned int) point.x;
    unsigned int b = (unsigned int) point.y;

    const unsigned int w = 8 * sizeof(unsigned int);
    const unsigned int s = w / 2;

    a *= 3284157443;
    b ^= a << s | a >> w - s;
    b *= 1911520717;
    a ^= b << s | b >> w - s;
    a *= 2048419325;

    float random = a * (M_PI / ~(~0u >> 1));

    return glm::vec2(cos(random), sin(random));
}

float Noise::interpolate(float a, float b, float shift) {
    return (b - a) * shift * shift * shift * (shift * (shift * 6.0f - 15.0f) + 10.0f) + a;
}
