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
    SDialog();
    SDialog(gfx::BHandle* dialogMesh, EDialogType type);

    void onFrame();

private:
    SAnimation animation;

    float positionStep[3];
    float scaleStep;

    void calcAnimationSteps();
    void calcWorldMatrix();
};

// -----------------------------------------------------------------------------

gfx::BHandle CreateDialogMesh(gfx::BHandle &material);