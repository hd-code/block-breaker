#include "game.hpp"

// -----------------------------------------------------------------------------

CGame::CGame(gfx::BHandle* ballMesh, gfx::BHandle* blockMesh, gfx::BHandle* paddleMesh)
    : status(EGameStatus::ON)
{
    this->ball = createBall(ballMesh);
    // this->entities.push_back(&this->ball);

    this->paddle = createPaddle(paddleMesh);
    this->entities.push_back(&this->paddle);

    for (int i = 0; i < NUM_OF_BED_ROCKS; i++) {
        float position[3];
        memcpy(position, BED_ROCKS[i].position, sizeof(float)*3);
        this->bedRocks[i] = createBlock(blockMesh, BED_ROCKS[i].type, position);
        this->entities.push_back(&this->bedRocks[i]);
    }
    
    // this->startOfBlocks = this->entities.size();
    // for (int i = 0; i < NUM_OF_BLOCKS; i++) {
    //     float position[3];
    //     memcpy(position, BLOCKS[i].position, sizeof(float)*3);
    //     this->blocks[i] = createBlock(blockMesh, BLOCKS[i].type, position);
    //     this->entities.push_back(&this->blocks[i]);
    // }
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
        // if (key == EKey::SPACE) {
        //     this->status = EGameStatus::ON;
        // }
        break;

    case EGameStatus::ON:
        this->advanceGame(key);
        // if (key == EKey::SPACE) {
        //     this->status = EGameStatus::PAUSED;
        // }
        break;

    case EGameStatus::WIN:
        break;

    case EGameStatus::LOST:
        break;

    }
}

// -----------------------------------------------------------------------------

const float LEFT_BORDER  = -5.0f;
const float RIGHT_BORDER =  5.0f;

const float BOTTOM_BORDER =  -5.0f;

void CGame::advanceGame(EKey key) {
    this->paddle.move(key, LEFT_BORDER, RIGHT_BORDER);
    this->handleCollisions();
    this->ball.move();

    // if (this->isWin()) {
    //     this->status = EGameStatus::WIN;
    // }

    // if (this->isLoss()) {
    //     this->status = EGameStatus::LOST;
    // }
}

void CGame::handleCollisions() {
    for (size_t i = 0; i < NUM_OF_BED_ROCKS; i++) {
        this->ball.handleCollision(this->bedRocks[i]);
    }
    
    for (size_t i = this->startOfBlocks; i < this->entities.size(); i++) {
        SBlock* block = (SBlock*)(this->entities[i]);
        bool collided = this->ball.handleCollision(*block);
        
        if (!collided) {
            continue;
        }

        switch (block->type) {
        case EBlockType::NORMAL:
            this->entities.erase(this->entities.begin() + i);

        case EBlockType::HARD:
            if (block->hits >= HARD_BLOCK_NUM_OF_HITS) {
                this->entities.erase(this->entities.begin() + i);
            }
        }
    }
}

bool CGame::isLoss() {
    return this->ball.isOnGround(BOTTOM_BORDER);
}

bool CGame::isWin() {
    return this->startOfBlocks >= this->entities.size();
}