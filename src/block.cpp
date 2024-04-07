#include "block.hpp"
#include "glm/glm.hpp"

Block::Block() {
    type = AIR;
}

Block::Block(BlockType type) {
    this->type = type;
}

glm::vec3 Block::color() {
    switch (type) {
        case GRASS:
            return glm::vec3(0.0f, 0.5f, 0.0f);
        case DIRT:
            return glm::vec3(0.6f, 0.3f, 0.2f);
        case STONE:
            return glm::vec3(0.5f, 0.5f, 0.5f);
        default:
            return glm::vec3(0.0f, 0.0f, 0.0f);
    }
}
