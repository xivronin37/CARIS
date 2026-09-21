#pragma once

#include "init.h"
#include "economy.h"
#include "gov.h"
#include "industry.h"

class Simulation {
    private:
        Economy& economy;
        Government& government;
        Industries& industries;
        int pauseInterval = 6;
        int tickNumber = 0;
    public:
        void tick();
        void run();
        Simulation(Economy& econ, Government& gov, Industries& ind);
};

std::string formatNumber(double value);