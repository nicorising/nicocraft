#include "block.hpp"
#include "glm/glm.hpp"

Block::Block() {
    type = AIR;
}

Block::Block(BlockType type) {
    this->type = type;
}

glm::vec4 Block::color() {
    switch (type) {
        case GRASS:
            return glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
        case DIRT:
            return glm::vec4(0.6f, 0.3f, 0.2f, 1.0f);
        case STONE:
            return glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        default:
            return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}
