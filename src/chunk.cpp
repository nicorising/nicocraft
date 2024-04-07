#include "block.hpp"
#include "chunk.hpp"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <iostream>

Chunk::Chunk() {
    for (unsigned int x = 0; x < size; x++) {
        for (unsigned int y = 0; y < size; y++) {
            for (unsigned int z = 0; z < size; z++) {
                blocks[x][y][z] = Block(AIR);
            }
        }
    }
}

void Chunk::setBlock(unsigned int x, unsigned int y, unsigned int z, BlockType type) {
    blocks[x][y][z].type = type;
}

unsigned int Chunk::updateBuffer(unsigned int VBO) {
    std::vector<float> vertices;

    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            for (int z = 0; z < size; z++) {
                Block block = blocks[x][y][z];
                glm::vec3 color = block.color();

                if (block.type != AIR) {
                    if (x + 1 >= size || blocks[x + 1][y][z].type == AIR) { // Add right face
                        for (unsigned int v = 0; v < 6; v++) {
                            glm::vec3 vertex = Block::rightFace[v];
                            vertices.push_back(vertex.x + x);
                            vertices.push_back(vertex.y + y);
                            vertices.push_back(vertex.z + z);
                            vertices.push_back(color.x);
                            vertices.push_back(color.y);
                            vertices.push_back(color.z);
                        }
                    }
                    if (x - 1 < 0 || blocks[x - 1][y][z].type == AIR) { // Add left face
                        for (unsigned int v = 0; v < 6; v++) {
                            glm::vec3 vertex = Block::leftFace[v];
                            vertices.push_back(vertex.x + x);
                            vertices.push_back(vertex.y + y);
                            vertices.push_back(vertex.z + z);
                            vertices.push_back(color.x);
                            vertices.push_back(color.y);
                            vertices.push_back(color.z);
                        }
                    }
                    if (y + 1 >= size || blocks[x][y + 1][z].type == AIR) { // Add top face
                        for (unsigned int v = 0; v < 6; v++) {
                            glm::vec3 vertex = Block::topFace[v];
                            vertices.push_back(vertex.x + x);
                            vertices.push_back(vertex.y + y);
                            vertices.push_back(vertex.z + z);
                            vertices.push_back(color.x);
                            vertices.push_back(color.y);
                            vertices.push_back(color.z);
                        }
                    }
                    if (y - 1 < 0 || blocks[x][y - 1][z].type == AIR) { // Add bottom face
                        for (unsigned int v = 0; v < 6; v++) {
                            glm::vec3 vertex = Block::bottomFace[v];
                            vertices.push_back(vertex.x + x);
                            vertices.push_back(vertex.y + y);
                            vertices.push_back(vertex.z + z);
                            vertices.push_back(color.x);
                            vertices.push_back(color.y);
                            vertices.push_back(color.z);
                        }
                    }
                    if (z + 1 >= size || blocks[x][y][z + 1].type == AIR) { // Add back face
                        for (unsigned int v = 0; v < 6; v++) {
                            glm::vec3 vertex = Block::backFace[v];
                            vertices.push_back(vertex.x + x);
                            vertices.push_back(vertex.y + y);
                            vertices.push_back(vertex.z + z);
                            vertices.push_back(color.x);
                            vertices.push_back(color.y);
                            vertices.push_back(color.z);
                        }
                    }
                    if (z - 1 < 0 || blocks[x][y][z - 1].type == AIR) { // Add front face
                        for (unsigned int v = 0; v < 6; v++) {
                            glm::vec3 vertex = Block::frontFace[v];
                            vertices.push_back(vertex.x + x);
                            vertices.push_back(vertex.y + y);
                            vertices.push_back(vertex.z + z);
                            vertices.push_back(color.x);
                            vertices.push_back(color.y);
                            vertices.push_back(color.z);
                        }
                    }
                }
            }
        }
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices[0]);
    return vertices.size() / 6;
}
