#include "init.h"
#include <cstdint>
#include <random>
#include <cmath>
#include <ctime>

double Init::normalize(double value, double min, double max)
{
    return (value - min) / (max - min);
}

void Init::init() {
    uint64_t initSeed = (inputSeed * ((1 - inputSeed) << 2)) >> 2;
    std::mt19937_64 rng(initSeed);
    double max = static_cast<double>(UINT64_MAX);

    randVal = normalize(static_cast<double>(rng()), 0, max);

    std::time_t now = std::time(nullptr);

    int randSeed = inputSeed ^ static_cast<int>(now);
    
    rand.seed(randSeed);
}

double Init::random() {

    double r = normalize(static_cast<double>(rand()), 0, static_cast<double>(UINT64_MAX));

    if (static_cast<int>(r) % 2 != 0) {
        r = normalize(static_cast<double>(rand()), 0, static_cast<double>(UINT64_MAX));
    }

    return r;
}