#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

class Camera {
public:
    glm::vec3 position;

    Camera(float aspectRatio, float nearPlane, float farPlane, glm::vec3 position, float movementSpeed, float mouseSpeed, float scrollSpeed);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    void processMovement(glm::vec3 direction, float deltaTime);
    void processMouse(float xOffset, float yOffset);
    void processScroll(float scrollOffset);
private:
    const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    float aspectRatio;
    float nearPlane;
    float farPlane;

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
