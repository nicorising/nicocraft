#ifndef WORLD_H
#define WORLD_H

#include "chunk.hpp"
#include "noise.hpp"

#include "glm/glm.hpp"

#include <map>

struct ivec3Compare {
    bool operator()(const glm::ivec3 a, const glm::ivec3 b) const {
        if (a.x != b.x) {
            return a.x < b.x;
        }

        if (a.y != b.y) {
            return a.y < b.y;
        }

        return a.z < b.z;
    }
};

class World {
public:
    World();
    Chunk getChunk(int x, int y, int z);
    void generate(int x, int y, int z);
private:
    std::map<glm::ivec3, Chunk, ivec3Compare> chunks;
    Noise noise;
};

#endif
