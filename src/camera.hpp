#pragma once

struct SCamera {
    float position[3] = { 0.0f, 0.0f,-8.0f };
    float target[3]   = { 0.0f, 0.0f, 0.0f };
    float up[3]       = { 0.0f, 1.0f, 0.0f };
    float aperture = 60.0f;
    float nearClip = 0.1f;
    float farClip  = 50.0f;
};

struct SLight {
    float direction[3] = { -1.0f, -0.7f, 2.0f };
    float ambient[3] = { 0.1f, 0.1f, 0.1f };
};