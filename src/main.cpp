#include "app.hpp"

#include "yoshix.h"

// -----------------------------------------------------------------------------

void main()
{
    CApplication app;
    gfx::RunApplication(800, 600, "BlockBreaker HD", &app);
}