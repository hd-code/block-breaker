#pragma once

#include "../entities/block.hpp"

// -----------------------------------------------------------------------------

struct SBlockList {
    EBlockType type;
    float position[3];
};

const unsigned int NUM_OF_BED_ROCKS = 27;

const SBlockList BED_ROCKS[] = {
    { EBlockType::BED_ROCK, -3.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK, -2.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK, -1.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK,  0.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK,  1.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK,  2.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK,  3.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK, -3.0f, 4.0f, 0.0f },
    { EBlockType::BED_ROCK,  3.0f, 4.0f, 0.0f },
    { EBlockType::BED_ROCK, -3.0f, 3.0f, 0.0f },
    { EBlockType::BED_ROCK,  3.0f, 3.0f, 0.0f },
    { EBlockType::BED_ROCK, -3.0f, 2.0f, 0.0f },
    { EBlockType::BED_ROCK,  3.0f, 2.0f, 0.0f },
    { EBlockType::BED_ROCK, -3.0f, 1.0f, 0.0f },
    { EBlockType::BED_ROCK,  3.0f, 1.0f, 0.0f },
    { EBlockType::BED_ROCK, -3.0f, 0.0f, 0.0f },
    { EBlockType::BED_ROCK,  3.0f, 0.0f, 0.0f },
    { EBlockType::BED_ROCK, -3.0f,-1.0f, 0.0f },
    { EBlockType::BED_ROCK,  3.0f,-1.0f, 0.0f },
    { EBlockType::BED_ROCK, -3.0f,-2.0f, 0.0f },
    { EBlockType::BED_ROCK,  3.0f,-2.0f, 0.0f },
    { EBlockType::BED_ROCK, -3.0f,-3.0f, 0.0f },
    { EBlockType::BED_ROCK,  3.0f,-3.0f, 0.0f },
    { EBlockType::BED_ROCK, -3.0f,-4.0f, 0.0f },
    { EBlockType::BED_ROCK,  3.0f,-4.0f, 0.0f },
    { EBlockType::BED_ROCK, -3.0f,-5.0f, 0.0f },
    { EBlockType::BED_ROCK,  3.0f,-5.0f, 0.0f },
};


const unsigned int NUM_OF_BLOCKS = 15;

const SBlockList BLOCKS[] = {
    { EBlockType::NORMAL, -2.0f, 3.0f, 0.0f },
    { EBlockType::HARD,   -1.0f, 3.0f, 0.0f },
    { EBlockType::NORMAL,  0.0f, 3.0f, 0.0f },
    { EBlockType::HARD,    1.0f, 3.0f, 0.0f },
    { EBlockType::NORMAL,  2.0f, 3.0f, 0.0f },

    { EBlockType::HARD,   -2.0f, 2.0f, 0.0f },
    { EBlockType::NORMAL, -1.0f, 2.0f, 0.0f },
    { EBlockType::HARD,    0.0f, 2.0f, 0.0f },
    { EBlockType::NORMAL,  1.0f, 2.0f, 0.0f },
    { EBlockType::HARD,    2.0f, 2.0f, 0.0f },

    { EBlockType::NORMAL, -2.0f, 1.0f, 0.0f },
    { EBlockType::HARD,   -1.0f, 1.0f, 0.0f },
    { EBlockType::NORMAL,  0.0f, 1.0f, 0.0f },
    { EBlockType::HARD,    1.0f, 1.0f, 0.0f },
    { EBlockType::NORMAL,  2.0f, 1.0f, 0.0f },
};