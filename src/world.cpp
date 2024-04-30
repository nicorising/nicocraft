#include "block.hpp"
#include "chunk.hpp"
#include "noise.hpp"
#include "world.hpp"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <iostream>

World::World() {}

Chunk World::getChunk(int x, int y, int z) {
    glm::ivec3 coord = glm::ivec3(x, y, z);

    if (!chunks.contains(coord)) {
        generate(x, y, z);
    }

    return chunks[glm::ivec3(x, y, z)];
}

void World::generate(int x, int y, int z) {
    Chunk chunk = Chunk();

    for (unsigned int blockX = 0; blockX < Chunk::size; blockX++) {
        for (unsigned int blockZ = 0; blockZ < Chunk::size; blockZ++) {
            float noiseX = (float) blockX / Chunk::size + x;
            float noiseY = (float) blockZ / Chunk::size + z;
            unsigned int height = (noise.noise(noiseX, noiseY, 1.0f) * 0.7f +
                                   noise.noise(noiseX, noiseY, 2.0f) * 0.2f +
                                   noise.noise(noiseX, noiseY, 4.0f) * 0.1f) * Chunk::size;

            for (unsigned blockY = 0; blockY < height; blockY++) {
                BlockType type;

                if (blockY < height - 3) {
                    type = STONE;
                } else if (blockY < height - 1) {
                    type = DIRT;
                } else {
                    type = GRASS;
                }
                
                chunk.setBlock(blockX, blockY, blockZ, type);
            }
        }
    }

    chunk.updateMesh();
    chunks[glm::ivec3(x, y, z)] = chunk;
}
