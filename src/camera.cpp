#include "camera.hpp"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(float aspectRatio, float nearPlane, float farPlane, glm::vec3 position, float movementSpeed, float mouseSpeed, float scrollSpeed) {
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    this->position = position;

    this->yaw = -90.0f;
    this->pitch = 0.0f;
    this->fov = 45.0f;

    this->movementSpeed = movementSpeed;
    this->mouseSpeed = mouseSpeed;
    this->scrollSpeed = scrollSpeed;

    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::processMovement(glm::vec3 direction, float deltaTime) {
    if (glm::length(direction)) {
        direction = glm::normalize(direction) * movementSpeed;
        position += glm::normalize(glm::cross(up, right)) * direction.x;
        position += right * direction.y;
        position += up * direction.z;
    }
}

void Camera::processMouse(float xOffset, float yOffset) {
    yaw += xOffset * mouseSpeed;
    pitch += yOffset * mouseSpeed;

    if (pitch > 89.0f) {
        pitch =  89.0f;
    } else if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processScroll(float scrollOffset) {
    fov -= scrollOffset * scrollSpeed;

    if (fov < 1.0f) {
        fov = 1.0f;
    } else if (fov > 45.0f) {
        fov = 45.0f; 
    }
}

void Camera::updateCameraVectors() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, up));
}
