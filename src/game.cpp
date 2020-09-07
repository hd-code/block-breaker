#include "game.hpp"

// -----------------------------------------------------------------------------

CGame::CGame() {
    createBlock()
}

CGame::~CGame() {

}

void CGame::onUpdate(EKey key) {
    switch (this->status) {
    case EGameStatus::PAUSED:
        if (key == EKey::SPACE) {
            this->status = EGameStatus::ON;
        }
        break;

    case EGameStatus::ON:
        if (key == EKey::SPACE) {
            this->status = EGameStatus::PAUSED;
            break;
        }
        break;

    case EGameStatus::WIN:
        break;

    case EGameStatus::LOST:
        break;

    }
}

// -----------------------------------------------------------------------------

std::vector<SEntity*> CGame::getDynamicEntities() {
    std::vector<SEntity*> entities;
    entities.push_back(&this->ball);
    entities.push_back(&this->paddle);
    return entities;
}

std::vector<SEntity*> CGame::getStaticEntities() {
    std::vector<SEntity*> entities;
    for (int i = 0; i < 21; i++) {
        entities.push_back(&this->bedRocks[i]);
    }
    for (int i = 0; i < 15; i++) {
        entities.push_back(&this->collBlock[i]);
    }
    return entities;
}