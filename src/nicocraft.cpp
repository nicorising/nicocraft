#include "camera.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader.hpp"
#include "stb_image.h"

#include <cmath>
#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

const int windowWidth = 800;
const int windowHeight = 600;
const float nearPlane = 0.1f;
const float farPlane = 100.0f;

bool wireframe = false;

float lastMouseX = windowWidth / 2;
float lastMouseY = windowHeight / 2;
float mouseXOffset = 0.0f;
float mouseYOffset = 0.0f;
float scrollOffset = 0.0f;

float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
Camera camera = Camera(aspectRatio, nearPlane, farPlane, glm::vec3(0.0f, 0.0f, 3.0f), 0.2f, 0.3f, 1.8f);
glm::vec3 moveDir = glm::vec3(0.0f, 0.0f, 0.0f);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "NicoCraft", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initilize GLAD" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback); 

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);  

    Shader shader("shaders/shader.vs", "shaders/shader.fs");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f) 
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
        0, 1, 5,
        0, 4, 5,
        2, 3, 7,
        2, 6, 7,
        0, 3, 7,
        0, 4, 7,
        1, 2, 6,
        1, 5, 6
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data;
    stbi_set_flip_vertically_on_load(true);  

    data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float fov = 45.0f;

    float deltaTime = 0.0f;
    float lastFrame = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();

        shader.setMat4("view", camera.getViewMatrix());

        shader.setMat4("projection", camera.getProjectionMatrix());

        glBindVertexArray(VAO);

        for(unsigned int i = 0; i < 10; i++) {
            float angle = 20.0f * (float) glfwGetTime() + i * 20.0f; 

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        camera.processMovement(moveDir, deltaTime);
        camera.processMouse(mouseXOffset, mouseYOffset);
        camera.processScroll(scrollOffset);

        mouseXOffset = 0.0f;
        mouseYOffset = 0.0f;
        scrollOffset = 0.0f;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(window, true);
                break;
            case GLFW_KEY_W:
                moveDir.x += 1.0f;
                break;
            case GLFW_KEY_S:
                moveDir.x -= 1.0f;
                break;
            case GLFW_KEY_D:
                moveDir.y += 1.0f;
                break;
            case GLFW_KEY_A:
                moveDir.y -= 1.0f;
                break;
            case GLFW_KEY_SPACE:
                moveDir.z += 1.0f;
                break;
            case GLFW_KEY_LEFT_SHIFT:
                moveDir.z -= 1.0f;
                break;
            case GLFW_KEY_X:
                glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_FILL : GL_LINE);
                wireframe = !wireframe;
        }
    } else if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_W:
                moveDir.x -= 1.0f;
                break;
            case GLFW_KEY_S:
                moveDir.x += 1.0f;
                break;
            case GLFW_KEY_D:
                moveDir.y -= 1.0f;
                break;
            case GLFW_KEY_A:
                moveDir.y += 1.0f;
                break;
            case GLFW_KEY_SPACE:
                moveDir.z -= 1.0f;
                break;
            case GLFW_KEY_LEFT_SHIFT:
                moveDir.z += 1.0f;
        }
    }
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
    mouseXOffset = xPos - lastMouseX;
    mouseYOffset = lastMouseY - yPos; // Invert y-offset

    lastMouseX = xPos;
    lastMouseY = yPos;
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    scrollOffset = yOffset;
}
