#include "entity.hpp"

#include "yoshix.h"

// -----------------------------------------------------------------------------

void SEntity::updateWorldMatrix() {
     gfx::GetTranslationMatrix(
        this->position[0],
        this->position[1],
        this->position[2],
        this->worldMatrix
    );
}