#include "math.hpp"

#include <cstdlib>

// -----------------------------------------------------------------------------

float GetRandom() {
    return float(std::rand()) / float(RAND_MAX);
}

float GetRandom(float min, float max) {
    float diff = max - min;
    return GetRandom() * diff + min;
}

void SetSeed(unsigned int seed) {
    std::srand(seed);
}