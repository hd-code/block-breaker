#pragma once

#include "entities/ball.hpp"
#include "entities/block.hpp"
#include "entities/entity.hpp"
#include "entities/paddle.hpp"
#include "key.hpp"

#include <vector>

// -----------------------------------------------------------------------------

enum EGameStatus { PAUSED, ON, WIN, LOST };

class CGame {
public:
    CGame();
    ~CGame();

    void onUpdate(EKey key);

    std::vector<SEntity*> getDynamicEntities();
    std::vector<SEntity*> getStaticEntities();

private:
    EGameStatus status;

    SBall ball;
    SPaddle paddle;
    SBlock bedRocks[21];
    SBlock collBlock[15];
};