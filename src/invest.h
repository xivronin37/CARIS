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
        double bondIntRate;

        Investor(Economy& economy, Government& government, Industries& industries);
        void InvestorInit();
        void update();
        void display();
        void buy(std::string target, double amount);
        void sell(std::string target, double amount);
        double totalValue();
};

class Portfolio {

};