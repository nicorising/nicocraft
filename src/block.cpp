#include "block.hpp"
#include "glm/glm.hpp"

#include <iostream>

Block::Block() {
    type = AIR;
}

Block::Block(BlockType type) {
    this->type = type;
}

glm::vec3 Block::getColor() {
    switch (type) {
        case GRASS:
            return colors[1];
        case DIRT:
            return colors[2];
        case STONE:
            return colors[3];
        default:
            return colors[0];
    }
}

void Block::addFace(std::vector<float>& vertices, unsigned int faceIdx, glm::vec3 offset) {
    glm::vec3 color = getColor();
    glm::vec3 normal = normals[faceIdx];

    for (unsigned int v = 0; v < 6; v++) {
        glm::vec3 vertex = faces[faceIdx][v] + offset;
        vertices.push_back(vertex.x);
        vertices.push_back(vertex.y);
        vertices.push_back(vertex.z);

        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(normal.z);

        vertices.push_back(color.x);
        vertices.push_back(color.y);
        vertices.push_back(color.z);
    }
}
