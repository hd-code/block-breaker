#pragma once

// -----------------------------------------------------------------------------

// The defaults are used to set the camera settings. This is easier to read and
// understand, in contrast to having a bunch of constants.
struct SCamera {
    float position[3] = { 0.0f, 0.0f,-10.0f };
    float target[3]   = { 0.0f, 0.0f,  0.0f };
    float up[3]       = { 0.0f, 1.0f,  0.0f };
    float aperture = 60.0f;
    float nearClip = 0.1f;
    float farClip  = 50.0f;
};