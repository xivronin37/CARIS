#pragma once

#include "init.h"
#include "economy.h"

class Government {;
    public:
        double taxRate = 0.15;
        double governmentSpending = 0;
        double debt = 0;
        double debtRatio;
        double targetDebt = 0.5;
        double taxAdjust = 0.01;
        double intervalAdjust = 12;
        double interestRate;
        int monthsWorse = 0;
        void update(Economy& economy);
        void govStats();
        Government(Init& init, Economy& economy);
};