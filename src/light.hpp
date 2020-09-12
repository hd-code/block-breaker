#pragma once

// -----------------------------------------------------------------------------

// The defaults are used to set the light settings. This is easier to read and
// understand, in contrast to having a bunch of constants.
struct SLight {
    float direction[3] = { -1.0f, -0.7f, 4.0f };
    float ambient[3] = { 0.2f, 0.2f, 0.2f };
};