#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include "glm/glm.hpp"

class Camera {
public:
    Camera(float aspectRatio, float nearPlane, float farPlane, glm::vec3 position, float movementSpeed, float mouseSpeed, float scrollSpeed);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    void processMovement(glm::vec3 direction, float deltaTime);
    void processMouse(float xOffset, float yOffset);
    void processScroll(float scrollOffset);
private:
    float aspectRatio;
    float nearPlane;
    float farPlane;

    const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 right;

    float movementSpeed;
    float mouseSpeed;
    float scrollSpeed;

    float yaw;
    float pitch;
    float fov;

    void updateCameraVectors();
};

#endif
