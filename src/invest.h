#pragma once

#include "init.h"
#include "economy.h"
#include "gov.h"
#include "industry.h"


class Investor {    
    private:
        Economy& econ;
        Government& gov;
        Industries& ind;
        double baseRate = 0.03;
    public:
        double cash;
        double agricultureShares;
        double manufacturingShares;
        double constructionShares;
        double energyShares;
        double servicesShares;
        double indexShares;
        double bondPrincipal;
        double interestRate;

        Investor(Economy& economy, Government& government, Industries& industries);
        void InvestorInit();
        void update();
};

class Portfolio {

};