#pragma once

#include "../entity/block.hpp"

// -----------------------------------------------------------------------------

const float BORDER_TOP    =  4.5f;
const float BORDER_BOTTOM = -4.5f;
const float BORDER_RIGHT  =  3.5f;
const float BORDER_LEFT   = -3.5f;

// -----------------------------------------------------------------------------

struct SBlockList {
    EBlockType type;
    float position[3];
};

const unsigned int NUM_OF_BED_ROCKS = 29;
static SBlockList BED_ROCKS[] = {
    // top row
    { EBlockType::BED_ROCK, -4.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK, -3.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK, -2.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK, -1.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK,  0.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK,  1.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK,  2.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK,  3.0f, 5.0f, 0.0f },
    { EBlockType::BED_ROCK,  4.0f, 5.0f, 0.0f },
    // left and right blocks
    { EBlockType::BED_ROCK, -4.0f, 4.0f, 0.0f },
    { EBlockType::BED_ROCK,  4.0f, 4.0f, 0.0f },
    { EBlockType::BED_ROCK, -4.0f, 3.0f, 0.0f },
    { EBlockType::BED_ROCK,  4.0f, 3.0f, 0.0f },
    { EBlockType::BED_ROCK, -4.0f, 2.0f, 0.0f },
    { EBlockType::BED_ROCK,  4.0f, 2.0f, 0.0f },
    { EBlockType::BED_ROCK, -4.0f, 1.0f, 0.0f },
    { EBlockType::BED_ROCK,  4.0f, 1.0f, 0.0f },
    { EBlockType::BED_ROCK, -4.0f, 0.0f, 0.0f },
    { EBlockType::BED_ROCK,  4.0f, 0.0f, 0.0f },
    { EBlockType::BED_ROCK, -4.0f,-1.0f, 0.0f },
    { EBlockType::BED_ROCK,  4.0f,-1.0f, 0.0f },
    { EBlockType::BED_ROCK, -4.0f,-2.0f, 0.0f },
    { EBlockType::BED_ROCK,  4.0f,-2.0f, 0.0f },
    { EBlockType::BED_ROCK, -4.0f,-3.0f, 0.0f },
    { EBlockType::BED_ROCK,  4.0f,-3.0f, 0.0f },
    { EBlockType::BED_ROCK, -4.0f,-4.0f, 0.0f },
    { EBlockType::BED_ROCK,  4.0f,-4.0f, 0.0f },
    { EBlockType::BED_ROCK, -4.0f,-5.0f, 0.0f },
    { EBlockType::BED_ROCK,  4.0f,-5.0f, 0.0f },
};

const unsigned int NUM_OF_BLOCKS = 21;
static SBlockList BLOCKS[] = {
    { EBlockType::HARD,   -3.0f, 3.0f, 0.0f },
    { EBlockType::NORMAL, -2.0f, 3.0f, 0.0f },
    { EBlockType::HARD,   -1.0f, 3.0f, 0.0f },
    { EBlockType::NORMAL,  0.0f, 3.0f, 0.0f },
    { EBlockType::HARD,    1.0f, 3.0f, 0.0f },
    { EBlockType::NORMAL,  2.0f, 3.0f, 0.0f },
    { EBlockType::HARD,    3.0f, 3.0f, 0.0f },

    { EBlockType::NORMAL, -3.0f, 2.0f, 0.0f },
    { EBlockType::HARD,   -2.0f, 2.0f, 0.0f },
    { EBlockType::NORMAL, -1.0f, 2.0f, 0.0f },
    { EBlockType::HARD,    0.0f, 2.0f, 0.0f },
    { EBlockType::NORMAL,  1.0f, 2.0f, 0.0f },
    { EBlockType::HARD,    2.0f, 2.0f, 0.0f },
    { EBlockType::NORMAL,  3.0f, 2.0f, 0.0f },

    { EBlockType::HARD,   -3.0f, 1.0f, 0.0f },
    { EBlockType::NORMAL, -2.0f, 1.0f, 0.0f },
    { EBlockType::HARD,   -1.0f, 1.0f, 0.0f },
    { EBlockType::NORMAL,  0.0f, 1.0f, 0.0f },
    { EBlockType::HARD,    1.0f, 1.0f, 0.0f },
    { EBlockType::NORMAL,  2.0f, 1.0f, 0.0f },
    { EBlockType::HARD,    3.0f, 1.0f, 0.0f },
};