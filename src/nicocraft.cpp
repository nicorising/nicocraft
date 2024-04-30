#include "block.hpp"
#include "camera.hpp"
#include "chunk.hpp"
#include "noise.hpp"
#include "shader.hpp"
#include "world.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"

#include <cmath>
#include <iostream>
#include <map>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

const int windowWidth = 1400;
const int windowHeight = 1000;
const float nearPlane = 0.1f;
const float farPlane = 1000.0f;

bool wireframe = false;

float lastMouseX = windowWidth / 2;
float lastMouseY = windowHeight / 2;
float mouseXOffset = 0.0f;
float mouseYOffset = 0.0f;
float scrollOffset = 0.0f;

float aspectRatio = (float) windowWidth / (float) windowHeight;
glm::vec3 moveDir = glm::vec3(0.0f, 0.0f, 0.0f);

const int renderDistance = 3;

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

    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_DEPTH_TEST);  

    glEnable(GL_CULL_FACE);

    glClearColor(0.5f, 0.7f, 0.8f, 1.0f);
    glLineWidth(5.0f);

    Shader shader("shaders/shader.vs", "shaders/shader.fs");

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        Chunk::size * Chunk::size * Chunk::size * 3 * sizeof(Block::faces),
        NULL,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Camera camera = Camera(aspectRatio, nearPlane, farPlane, glm::vec3(0.0f, 20.0f, 0.0f), 0.4f, 0.3f, 1.8f);
    World world = World();

    shader.use();
    shader.setVec3("sunColor", glm::vec3(1.0f, 1.0f, 1.0f));

    float deltaTime = 0.0f;
    float lastFrame = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(VAO);
        shader.use();

        shader.setVec3("sunDir", glm::vec3(sin(glfwGetTime() / 100), -cos(glfwGetTime() / 100), 0.0f));

        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", camera.getProjectionMatrix());

        for (int dx = -renderDistance; dx <= renderDistance; dx++) {
            for (int dz = -renderDistance; dz <= renderDistance; dz++) {
                int x = floor(camera.position.x / Chunk::size) + dx;
                int z = floor(camera.position.z / Chunk::size) + dz;

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x, 0.0f, z) * (float) Chunk::size);
                shader.setMat4("model", model);

                Chunk chunk = world.getChunk(x, 0, z);
                unsigned int size = chunk.updateBuffer(VBO);
                glDrawArrays(GL_TRIANGLES, 0, size);
            }
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
                moveDir.z += 1.0f;
                break;
            case GLFW_KEY_S:
                moveDir.z -= 1.0f;
                break;
            case GLFW_KEY_D:
                moveDir.x += 1.0f;
                break;
            case GLFW_KEY_A:
                moveDir.x -= 1.0f;
                break;
            case GLFW_KEY_SPACE:
                moveDir.y += 1.0f;
                break;
            case GLFW_KEY_LEFT_SHIFT:
                moveDir.y -= 1.0f;
                break;
            case GLFW_KEY_X:
                glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_FILL : GL_LINE);
                wireframe = !wireframe;
        }
    } else if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_W:
                moveDir.z -= 1.0f;
                break;
            case GLFW_KEY_S:
                moveDir.z += 1.0f;
                break;
            case GLFW_KEY_D:
                moveDir.x -= 1.0f;
                break;
            case GLFW_KEY_A:
                moveDir.x += 1.0f;
                break;
            case GLFW_KEY_SPACE:
                moveDir.y -= 1.0f;
                break;
            case GLFW_KEY_LEFT_SHIFT:
                moveDir.y += 1.0f;
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
