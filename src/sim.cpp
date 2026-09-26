
#include <string>
#include <iostream>
#include <sstream>
#include "init.h"
#include "economy.h"
#include "gov.h"
#include "sim.h"
#include "invest.h"

void Simulation::tick() {
    government.update(economy);
    economy.update(government.governmentSpending);
    industries.update();
    economy.finalize(industries.totalOutput());
    investor.update();
    tickNumber++;
}

void Simulation::run() {
    std::cout << "How many ticks between pauses? ";
    std::cin >> pauseInterval;
    std::cin.ignore();
    while (true) {
        tick();
        if (tickNumber == 600) break;
        if (tickNumber % pauseInterval == 0) {
            std::cout << "Stopped at tick " << tickNumber << "\n";
            investor.display();
            std::string line;
            std::getline(std::cin, line);
            std::istringstream iss(line);
            std::string command, target;
            double amount;
            iss >> command >> target >> amount;

            if (command == "buy") {
            investor.buy(target, amount);
            } else if (command == "sell") {
                investor.sell(target, amount);
            } else if (command == "hold") {
                // do nothing, just continue
            } else if (command == "exit") {
                std::cout << "Simulation ran for " << formatNumber(tickNumber) << ((tickNumber > 1 || tickNumber == 0) ? " ticks" : " tick") << "\n";
                return;
            } else {
                std::cout << "Unrecognized command." << std::endl;
            }
        }
    }
}

Simulation::Simulation(Economy& econ, Government& gov, Industries& ind, Investor& inv) : economy(econ), government(gov), industries(ind), investor(inv) {
    Init init;
    init.init();
    econ.setEcon(init.randVal);
    ind.industryInit();
    econ.finalize(ind.totalOutput());
    inv.InvestorInit();
}