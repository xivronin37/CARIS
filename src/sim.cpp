
#include <string>
#include <iostream>
#include <sstream>
#include "init.h"
#include "economy.h"
#include "gov.h"
#include "sim.h"

void Simulation::tick() {
    government.update(economy);
    economy.update(government.governmentSpending);
    industries.update();
    economy.finalize(industries.totalOutput());
}

void Simulation::run() {
    while (true) {
        tick();
        tickNumber++;
        if (tickNumber == 600) break;
        /* if (tickNumber % pauseInterval == 0) {
            std::string input;
            std::cin >> input;
        } */
    }
}

Simulation::Simulation(Economy& econ, Government& gov, Industries& ind) : economy(econ), government(gov), industries(ind) {
    Init init;
    init.init();
    econ.setEcon(init.randVal);
    ind.industryInit();
    econ.finalize(ind.totalOutput());
}