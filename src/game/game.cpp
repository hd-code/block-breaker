#include "game.hpp"

// -----------------------------------------------------------------------------

CGame::CGame(gfx::BHandle* ballMesh, gfx::BHandle* blockMesh, gfx::BHandle* paddleMesh)
    : status(EGameStatus::PAUSED)
{
    this->ball = CreateBall(ballMesh);
    this->entities.push_back(&this->ball);

    this->paddle = CreatePaddle(paddleMesh);
    this->entities.push_back(&this->paddle);

    for (int i = 0; i < NUM_OF_BED_ROCKS; i++) {
        float position[3];
        memcpy(position, BED_ROCKS[i].position, sizeof(float)*3);
        this->bedRocks[i] = CreateBlock(blockMesh, BED_ROCKS[i].type, position);
        this->entities.push_back(&this->bedRocks[i]);
    }
    
    this->startOfBlocks = this->entities.size();
    for (int i = 0; i < NUM_OF_BLOCKS; i++) {
        float position[3];
        memcpy(position, BLOCKS[i].position, sizeof(float)*3);
        this->blocks[i] = CreateBlock(blockMesh, BLOCKS[i].type, position);
        this->entities.push_back(&this->blocks[i]);
    }
}

CGame::~CGame() {
    this->entities.clear();
}

std::vector<SEntity*>* CGame::getEntities() {
    return &this->entities;
}

void CGame::onUpdate(EKey key) {
    switch (this->status) {
    case EGameStatus::PAUSED:
        if (key == EKey::SPACE) {
            this->status = EGameStatus::ON;
        }
        break;

    case EGameStatus::ON:
        this->advanceGame(key);
        if (key == EKey::SPACE) {
            this->status = EGameStatus::PAUSED;
        }
        break;

    case EGameStatus::WIN:
        break;

    case EGameStatus::LOST:
        break;

    }
}

// -----------------------------------------------------------------------------

void CGame::advanceGame(EKey key) {
    this->paddle.move(key, BORDER_LEFT, BORDER_RIGHT);
    this->handleCollisions();
    this->ball.move();

    // if (this->isWin()) {
    //     this->status = EGameStatus::WIN;
    // }

    if (this->isLoss()) {
        this->status = EGameStatus::LOST;
    }
}

void CGame::handleCollisions() {
    this->ball.handleCollision(BORDER_TOP, BORDER_LEFT, BORDER_RIGHT);

    this->ball.handleCollision(this->paddle);

    for (size_t i = this->startOfBlocks; i < this->entities.size(); i++) {
        SBlock* block = (SBlock*)(this->entities[i]);
        
        bool collided = this->ball.handleCollision(*block);
        
        if (collided) {
            if (block->isBroken()) {
                this->entities.erase(this->entities.begin() + i);
            }
            break;
        }
    }
}

bool CGame::isLoss() {
    return this->ball.isOnGround(BORDER_BOTTOM);
}

bool CGame::isWin() {
    return this->startOfBlocks >= this->entities.size();
}