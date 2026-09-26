#pragma once

#include "init.h"
#include "economy.h"
#include "gov.h"
#include "industry.h"
#include "invest.h"

class Simulation {
    private:
        Economy& economy;
        Government& government;
        Industries& industries;
        Investor& investor;
        int pauseInterval = 6;
    public:
        int tickNumber = 0;
        void tick();
        void run();
        Simulation(Economy& econ, Government& gov, Industries& ind, Investor& inv);
};

std::string formatNumber(double value);