#ifndef NOISE_H
#define NOISE_H

#include "glm/glm.hpp"

class Noise {
public:
    Noise();
    float noise(float x, float y);
private:
    glm::vec2 gradient(glm::vec2 point);
    float interpolate(float a, float b, float shift);
};

#endif

