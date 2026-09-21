
#include <cmath>
#include <deque>
#include <algorithm>
#include <iostream>
#include <numeric>
#include "industry.h"

double Industry::desiredEmployment(double wage) {
    return std::pow(0.7 * productivity / wage, 10.0 / 3.0);
}

void Industry::update() {
    employmentHistory.push_back(employment);
    while (employmentHistory.size() > 24) {
        employmentHistory.pop_front();
    }
    double low = *std::min_element(employmentHistory.begin(), employmentHistory.end());
    double high = *std::max_element(employmentHistory.begin(), employmentHistory.end());
    double target = (low + high) / 2.0;

    productivity += (employment - target) * 0.001;
    if (productivity < 2500) productivity = 2500;

    if (!calibrated) {
        valuationMultiple = 100.0 / output;
        calibrated = true;
        std::cout <<  "Valuation Multiple: " << valuationMultiple << "\n";
        std::cout << "Out:" << output << "\n";
    }

    outHistory.push_back(output);
    while (outHistory.size() > 24) {
        outHistory.pop_front();
    }

    double sum = std::accumulate(outHistory.begin(), outHistory.end(), 0.0);
    double avg = sum / outHistory.size();
    price = valuationMultiple * avg;
}

double Industry::outputVal(double productivity, double employment) {
    return productivity * std::pow(employment, 0.7);
}


void Industries::industryInit() {
    agriculture.update();
    manufacturing.update();
    construction.update();
    energy.update();
    services.update();

    agriculture.outputShare = 0.08;
    manufacturing.outputShare = 0.22;
    construction.outputShare = 0.07;
    energy.outputShare = 0.06;
    services.outputShare = 0.57;

    agriculture.productivity = 30000;
    manufacturing.productivity = 150000;
    construction.productivity = 60000;
    energy.productivity = 180000;
    services.productivity = 50000;

    agriculture.price = 100.0;
    manufacturing.price = 100.0;
    construction.price = 100.0;
    energy.price = 100.0;
    services.price = 100.0;

    indexPrice = 100.0;

    agriculture.output = econ.demandGDP * agriculture.outputShare;
    manufacturing.output = econ.demandGDP * manufacturing.outputShare;
    construction.output = econ.demandGDP * construction.outputShare;
    energy.output = econ.demandGDP * energy.outputShare;
    services.output = econ.demandGDP * services.outputShare;

}

void Industries::update() {
    double low = 0;
    double high = 1000000;
    double wage;
    while (true) {
        wage = (low + high) / 2;
        double workerSum = agriculture.desiredEmployment(wage) + manufacturing.desiredEmployment(wage) + construction.desiredEmployment(wage)
        + energy.desiredEmployment(wage) + services.desiredEmployment(wage);

        if (std::abs(workerSum - econ.laborForce) < 0.0001) break;
        else if (workerSum > econ.laborForce) low = wage;
        else high = wage;
    }

    agriculture.wage = wage;
    manufacturing.wage = wage;
    construction.wage = wage;
    energy.wage = wage;
    services.wage = wage;

    agriculture.employment = agriculture.desiredEmployment(wage);
    manufacturing.employment = manufacturing.desiredEmployment(wage);
    construction.employment = construction.desiredEmployment(wage);
    energy.employment = energy.desiredEmployment(wage);
    services.employment = services.desiredEmployment(wage);

    agriculture.output = agriculture.outputVal(agriculture.productivity, agriculture.employment);
    manufacturing.output = manufacturing.outputVal(manufacturing.productivity, manufacturing.employment);
    construction.output = construction.outputVal(construction.productivity, construction.employment);
    energy.output = energy.outputVal(energy.productivity, energy.employment);
    services.output = services.outputVal(services.productivity, services.employment);

    agriculture.update();
    manufacturing.update();
    construction.update();
    energy.update();
    services.update();

    double totalOut = totalOutput();
    double growthRate = hasPrevTotal ? (totalOut - previousTotal) / previousTotal : 0;
    indexPrice *= (1 + growthRate * 2.0);

    previousTotal = totalOut;
    hasPrevTotal = true;

    if (tickNumber % 120 == 0) {
        std::cout << "\n";
        std::cout << "AGR  prod=" << agriculture.productivity << " emp=" << agriculture.employment << " out=" << agriculture.output << " wage=" << agriculture.wage << " price=" << agriculture.price << "\n";
        std::cout << "MAN  prod=" << manufacturing.productivity << " emp=" << manufacturing.employment << " out=" << manufacturing.output << " wage=" << manufacturing.wage << " price=" << manufacturing.price << "\n";
        std::cout << "CON  prod=" << construction.productivity << " emp=" << construction.employment << " out=" << construction.output << " wage=" << construction.wage << " price=" << construction.price << "\n";
        std::cout << "ENE  prod=" << energy.productivity << " emp=" << energy.employment << " out=" << energy.output << " wage=" << energy.wage << " price=" << energy.price << "\n";
        std::cout << "SER  prod=" << services.productivity << " emp=" << services.employment << " out=" << services.output << " wage=" << services.wage << " price=" << services.price << "\n";
    }
    
    tickNumber++;
}

double Industries::totalOutput() {
    return agriculture.output + manufacturing.output + construction.output + energy.output + services.output;
}

Industries::Industries(Economy& economy, Government& government) : econ(economy), gov(government) {}