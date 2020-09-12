#pragma once

#include "yoshix.h"

// -----------------------------------------------------------------------------

gfx::BHandle CreateMaterial(
    int numOfTex, gfx::BHandle textures[],
    int numOfVSB, gfx::BHandle vsBuffers[],
    int numOfPSB, gfx::BHandle psBuffers[],
    int numOfInputs, gfx::SInputElement inputElements[],
    gfx::BHandle &vertexShader, gfx::BHandle &pixelShader);