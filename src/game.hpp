#pragma once

#include "data.hpp"
#include "entities/ball.hpp"
#include "entities/block.hpp"
#include "entities/entity.hpp"
#include "entities/paddle.hpp"
#include "key.hpp"
#include "yoshix.h"

#include <vector>

// -----------------------------------------------------------------------------

enum EGameStatus { PAUSED, ON, WIN, LOST };

class CGame {
public:
    CGame(gfx::BHandle* ballMesh, gfx::BHandle* blockMesh, gfx::BHandle* paddleMesh);
    ~CGame();

    std::vector<SEntity*>* getEntities();
    void onUpdate(EKey key);

private:
    void advanceGame(EKey key);

    void handleCollisions();

    bool isLoss();
    bool isWin();

private:
    EGameStatus status;

    SBall ball;
    SPaddle paddle;
    SBlock bedRocks[NUM_OF_BED_ROCKS];
    SBlock blocks[NUM_OF_BLOCKS];

    unsigned int startOfBlocks;

    std::vector<SEntity*> entities;
};