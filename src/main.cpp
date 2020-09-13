#include "app.hpp"
#include "helper/math.hpp"
#include "yoshix.h"

#include <ctime>

// -----------------------------------------------------------------------------

void main() {
    SetSeed(std::time(0));

    CApplication app;
    gfx::RunApplication(800, 600, "BlockBreaker HD", &app);
}