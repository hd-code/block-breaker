#include "entity.hpp"

#include "yoshix.h"

// -----------------------------------------------------------------------------

void updateWorldMatrix(SEntity &entity) {
    gfx::GetTranslationMatrix(
        entity.position[0],
        entity.position[1],
        entity.position[2],
        entity.worldMatrix
    );
}