#include "entities/ball.hpp"
#include "entities/block.hpp"
#include "entities/paddle.hpp"

#include <vector>

// -----------------------------------------------------------------------------

enum EGameStatus { PAUSED, ON, WIN, LOST };

class CGame {
public:
    CGame();
    CGame();

    std::vector<SEntity> getEntities();

private:
    std::vector<SEntity> entities;

    SBall* ball;
    SPaddle* paddle;
    SBlock* bedRocks[21];
    SBlock* collBlock[15];
};