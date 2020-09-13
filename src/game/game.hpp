#pragma once

#include "data.hpp"
#include "../entity/ball.hpp"
#include "../entity/block.hpp"
#include "../entity/dialog.hpp"
#include "../entity/entity.hpp"
#include "../entity/paddle.hpp"
#include "../key.hpp"
#include "yoshix.h"

#include <vector>

// -----------------------------------------------------------------------------

enum class EGameStatus { START, ON, PAUSED, WIN, LOSS };

class CGame {
public:
    CGame(gfx::BHandle* ballMesh, gfx::BHandle* blockMesh, gfx::BHandle* dialogMesh, gfx::BHandle* paddleMesh);
    ~CGame();

    std::vector<SEntity*>* getEntities();
    void onUpdate(EKey key);

private:
    void advanceGame(EKey key);

    void handleCollisions();
    
    void initGame();

    bool isLoss();
    bool isWin();

    void showDialog();
    void removeDialog();

private:
    EGameStatus status;

    SBall ball;
    SPaddle paddle;

    bool dialogShown;
    SDialog dialog;

    SBlock bedRocks[NUM_OF_BED_ROCKS]; // just used to visualize the borders
    SBlock blocks[NUM_OF_BLOCKS]; // the actual blocks in the game
    unsigned int startOfBlocks; // index in entities, where the blocks start

    std::vector<SEntity*> entities;

    gfx::BHandle* ballMesh;
    gfx::BHandle* blockMesh;
    gfx::BHandle* dialogMesh;
    gfx::BHandle* paddleMesh;
};