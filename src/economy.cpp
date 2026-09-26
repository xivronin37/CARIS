#include <iostream>
#include <cmath>
#include "init.h"
#include "economy.h"
#include "helper.h"

Economy::Economy() {}

void Economy::setEcon(double modifier) {
    population = modifier * 5000000;
    double laborForce = population * ((modifier * modifier) - modifier + 1) / 1.1;
    laborParticipation = laborForce / population;

    productivity = 20000 + modifier * 180000;
    potentialGDP = productivity * std::pow(laborForce, 0.7);

    double consumptionRate = 0.55 + modifier * 0.2;
    consumption = potentialGDP * consumptionRate;

    double investmentRate = 0.15 + modifier * 0.15;
    investment = potentialGDP * investmentRate;

    double governmentRate = 0.10 + modifier * 0.20;
    governmentSpending = potentialGDP * governmentRate;

    demandGDP = consumption + investment + governmentSpending;
}

void Economy::finalize(double indOutput) {
    gdp = indOutput;
    double demandGrowth = 0;
    if (hasPrev) {
        demandGrowth = (demandGDP - prevDemand) / prevDemand;
        outputGrowth = (gdp - prevGdp) / prevGdp;
        inflation = 0.02 + (demandGrowth - outputGrowth) * 0.5;
        priceLevel *= (1 + inflation);
    } else {
        inflation = 0.02;
        priceLevel *= (1 + inflation);
        outputGrowth = 0.01;
    }
    prevDemand = demandGDP;
    prevGdp = gdp;
    hasPrev = true;
}

void Economy::update(double govSpending) {
    double rand1 = init.random();
    double rand2 = init.random();
    double growthRate = (-0.01 + 0.03 * rand1) / 12;
    double productivityGrowthRate = (-0.02 + 0.06 * rand2) / 12;

    population *= (1 + growthRate);
    population = static_cast<int>(population);
    productivity *= (1 + productivityGrowthRate);

    laborForce = population * ((rand1 * rand1) - rand1 + 1) / 1.1;
    laborParticipation = laborForce / population;
    potentialGDP = productivity * std::pow(laborForce, 0.7);
    
    double consumptionRate = 0.55 + rand2 * 0.2 - (0.5 * inflation);
    consumption = potentialGDP * consumptionRate;

    double investmentRate = 0.15 + rand2 * 0.15 - (0.75 * inflation);
    investment = potentialGDP * investmentRate;
    governmentSpending = govSpending;
}

void Economy::stats() {
    std::cout << "Economy\n";
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Population: " << formatNumber(population) << "\n";
    std::cout << "Labor Participation: " << formatNumber((laborParticipation * 100)) << "%" << "\n";
    std::cout << "Productivity: " << formatNumber(productivity) << "\n";
    std::cout << "Consumption: " << formatNumber(consumption) << "\n";
    std::cout << "Investment: " << formatNumber(investment) << "\n";
    std::cout << "GDP: " << formatNumber(gdp) << "\n";
    std::cout << "Inflation: " << formatNumber((inflation * 100)) << "%" << "\n";
    std::cout << "--------------------------------" << std::endl;
    std::cout << std::flush;
}