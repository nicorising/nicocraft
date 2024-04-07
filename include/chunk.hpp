#ifndef CHUNK_H
#define CHUNK_H

class Chunk {
public:
    static constexpr unsigned int size = 32;

    Block blocks[size][size][size];

    Chunk();
    void setBlock(unsigned int x, unsigned int y, unsigned int z, BlockType type);
    unsigned int updateBuffer(unsigned int VBO);
};

#endif

