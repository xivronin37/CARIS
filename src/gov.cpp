
#include <iostream>
#include "gov.h"
#include "helper.h"

void Government::update(Economy& economy) {;
    double gdp = economy.gdp;
    double taxRevenue = gdp * taxRate;
    double govBudget = taxRevenue - economy.governmentSpending;
    debt -= govBudget;
    if (debt < 0) debt = 0;
    debtRatio = debt / gdp;

    if (debtRatio > targetDebt && taxRate < 0.8) taxRate += taxAdjust;
    else if (debtRatio < targetDebt && taxRate > 0) taxRate -= taxAdjust;
    
    if (debtRatio > targetDebt) {
        monthsWorse++;
    } else {
        monthsWorse = 0;
        intervalAdjust = 12;
        taxRate = 0.01;
    }

    if (monthsWorse >= intervalAdjust && intervalAdjust >= 0) {
        taxAdjust += 0.005;
        monthsWorse = 0;
        if (intervalAdjust > 1) intervalAdjust--;
    }

    double spendingRate = 0.2 - economy.outputGrowth * 0.5;
    governmentSpending = economy.potentialGDP * spendingRate;
    economy.governmentSpending = governmentSpending;
}

void Government::govStats() {
    std::cout << "Government\n";
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Tax Rate: " << formatNumber(taxRate*100) << "%\n";
    std::cout << "Government Spending: $" << formatNumber(governmentSpending) << "\n";
    std::cout << "Debt: $" << formatNumber(debt) << "\n";
    std::cout << "Debt Ratio: " << formatNumber(debtRatio) << "\n";
    std::cout << "--------------------------------" << std::endl;
    std::cout << std::flush;
}

Government::Government(Init& init, Economy& economy) {
    taxRate = 0.15 + init.random() * 0.25;
    debt = 0;
    governmentSpending = 0;
}