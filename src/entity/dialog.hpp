#pragma once

#include "entity.hpp"
#include "yoshix.h"

// -----------------------------------------------------------------------------

enum class EDialogType { START, PAUSE, LOSS, WIN };

struct SAnimation {
    float position[3];
    float scale;
    unsigned int frame;
};

struct SDialog : SEntity {
    SAnimation animation;

    void calcAnimationSteps();

    void onFrame();

private:
    float positionStep[3];
    float scaleStep;

    void calcWorldMatrix();
};

// -----------------------------------------------------------------------------

SDialog CreateDialog(gfx::BHandle* dialogMesh, EDialogType type);

// -----------------------------------------------------------------------------

gfx::BHandle CreateDialogMesh(gfx::BHandle &material);