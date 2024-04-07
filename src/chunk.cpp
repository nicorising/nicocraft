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
                glm::vec3 offset = glm::vec3(x, y, z);

                if (block.type != AIR) {
                    if (x + 1 >= size || blocks[x + 1][y][z].type == AIR) { // Add right face
                        block.addFace(vertices, 0, offset);
                    }
                    if (x - 1 < 0 || blocks[x - 1][y][z].type == AIR) { // Add left face
                        block.addFace(vertices, 1, offset);
                    }
                    if (y + 1 >= size || blocks[x][y + 1][z].type == AIR) { // Add top face
                        block.addFace(vertices, 2, offset);
                    }
                    if (y - 1 < 0 || blocks[x][y - 1][z].type == AIR) { // Add bottom face
                        block.addFace(vertices, 3, offset);
                    }
                    if (z + 1 >= size || blocks[x][y][z + 1].type == AIR) { // Add back face
                        block.addFace(vertices, 4, offset);
                    }
                    if (z - 1 < 0 || blocks[x][y][z - 1].type == AIR) { // Add front face
                        block.addFace(vertices, 5, offset);
                    }
                }
            }
        }
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices[0]);
    return vertices.size() / 9;
}
