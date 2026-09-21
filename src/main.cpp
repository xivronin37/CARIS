
#include <iostream>
#include <chrono>
#include <thread>
#include "init.h"
#include "economy.h"
#include "gov.h"
#include "sim.h"
#include "industry.h"

int main() {
    Init init;
    Economy econ;
    Government gov(init, econ);
    Industries ind(econ, gov);
    Simulation sim(econ, gov, ind);

    int years = 50;
    std::cout << "Year 0\nEconomy\n";
    econ.stats();
    std::cout << "Government\n";
    gov.govStats();
    std::cout << "\nRunning simulation...\n";
    sim.run();
    std::cout << "Year 50\nEconomy\n";
    econ.stats();
    std::cout << "Government\n";
    gov.govStats();
    


    return 0;
}