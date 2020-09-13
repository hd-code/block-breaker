#include "game.hpp"

// -----------------------------------------------------------------------------

CGame::CGame(gfx::BHandle* ballMesh, gfx::BHandle* blockMesh, gfx::BHandle* dialogMesh, gfx::BHandle* paddleMesh)
    : status(EGameStatus::START)
    , dialogShown(true)
    , ballMesh(ballMesh)
    , blockMesh(blockMesh)
    , dialogMesh(dialogMesh)
    , paddleMesh(paddleMesh)
{
    this->initGame();
    this->showDialog();
}

CGame::~CGame() {
    this->entities.clear();
}

std::vector<SEntity*>* CGame::getEntities() {
    return &this->entities;
}

void CGame::onUpdate(EKey key) {
    if (this->dialogShown) {
        this->dialog.onFrame();
    }

    switch (this->status) {
    case EGameStatus::START:
        if (key == EKey::SPACE) {
            this->status = EGameStatus::ON;
            this->removeDialog();
        }
        break;

    case EGameStatus::PAUSED:
        if (key == EKey::SPACE) {
            this->status = EGameStatus::ON;
            this->removeDialog();
        }
        break;

    case EGameStatus::ON:
        this->advanceGame(key);

        if (key == EKey::SPACE) {
            this->status = EGameStatus::PAUSED;
            this->showDialog();
            break;
        }

        if (this->isLoss()) {
            this->status = EGameStatus::LOSS;
            this->showDialog();
            break;
        }

        if (this->isWin()) {
            this->status = EGameStatus::WIN;
            this->showDialog();
            break;
        }

        break;

    case EGameStatus::WIN:
    case EGameStatus::LOSS:
        if (key == EKey::SPACE) {
            this->removeDialog();
            this->initGame();
            this->status = EGameStatus::ON;
        }
        break;
    }
}

// -----------------------------------------------------------------------------

void CGame::advanceGame(EKey key) {
    this->paddle.move(key, BORDER_LEFT, BORDER_RIGHT);
    this->handleCollisions();
    this->ball.move();
}

void CGame::handleCollisions() {
    // borders
    this->ball.handleCollision(BORDER_TOP, BORDER_LEFT, BORDER_RIGHT);

    // paddle
    this->ball.handleCollision(this->paddle);

    // blocks
    for (size_t i = this->startOfBlocks; i < this->entities.size(); i++) {
        SBlock* block = (SBlock*)(this->entities[i]);        
        if (this->ball.handleCollision(*block)) {
            if (block->isBroken()) {
                this->entities.erase(this->entities.begin() + i);
            }
        }
    }
}

void CGame::initGame() {
    this->entities.clear();

    this->ball = SBall(ballMesh);
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

bool CGame::isLoss() {
    return this->ball.isOnGround(BORDER_BOTTOM);
}

bool CGame::isWin() {
    return this->startOfBlocks >= this->entities.size();
}

void CGame::showDialog() {
    EDialogType type;

    switch (this->status) {
    case EGameStatus::START:  type = EDialogType::START; break;
    case EGameStatus::PAUSED: type = EDialogType::PAUSE; break;
    case EGameStatus::LOSS:   type = EDialogType::LOSS;  break;
    case EGameStatus::WIN:    type = EDialogType::WIN;   break;
    default: this->removeDialog(); return;
    }

    this->dialog = CreateDialog(this->dialogMesh, type);
    this->dialogShown = true;
    this->entities.push_back(&this->dialog);
}

void CGame::removeDialog() {
    if (this->dialogShown) {
        this->dialogShown = false;
        this->entities.pop_back();
    }
}