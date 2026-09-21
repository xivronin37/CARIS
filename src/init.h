#pragma once

#include <cstdint>
#include <random>

class Init {
    private:
        uint64_t inputSeed;
        double seed;
        std::mt19937_64 rand;
    public:
        double randVal;
        void init();
        double random();
        double normalize(double value, double min, double max);

};

