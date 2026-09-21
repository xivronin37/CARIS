#pragma once

#include <cstdint>
#include "init.h"

class Economy {
    private:
        Init init;
    public:
        double population;
        double laborForce;
        double laborParticipation;
        double productivity;
        double consumption;
        double investment;
        double governmentSpending;
        double potentialGDP;
        double demandGDP;
        double gdp;
        double inflation;
        double priceLevel = 1.0;
        
        Economy();
        void setEcon(double modifier);
        void finalize(double indOutput);
        void update(double govSpending);
        void stats();
};

