
#include <iostream>
#include "init.h"
#include "helper.h"
#include "economy.h"
#include "gov.h"
#include "industry.h"
#include "invest.h"

Investor::Investor(Economy& economy, Government& government, Industries& industries) : econ(economy), gov(government), ind(industries) {}

void Investor::InvestorInit() {
    cash = 10000.0;
    agricultureShares = 0;
    manufacturingShares = 0;
    constructionShares = 0;
    energyShares = 0;
    servicesShares = 0;
    indexShares = 0;
    bondPrincipal = 0;
}

void Investor::update() {
    bondIntRate = 0.03 + gov.debtRatio * 0.04;
}

void Investor::display() {
    std::cout << "Stats" << "\n";
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Cash: $" << formatNumber(cash) << "\n";
    double agr = agricultureShares * ind.agriculture.price;
    double man = manufacturingShares * ind.manufacturing.price;
    double con = constructionShares * ind.construction.price;
    double ene = energyShares * ind.energy.price;
    double ser = servicesShares * ind.services.price;
    double idx = indexShares * ind.indexPrice;
    std::cout << "Agriculture: " << formatNumber(agricultureShares) << " shares @ $" << formatNumber(ind.agriculture.price) <<
    " = $" << formatNumber(agr) << "\n";
    std::cout << "Manufacturing: " << formatNumber(manufacturingShares) << " shares @ $" << formatNumber(ind.manufacturing.price) <<
    " = $" << formatNumber(man) << "\n";
    std::cout << "Construction: " << formatNumber(constructionShares) << " shares @ $" << formatNumber(ind.construction.price) <<
    " = $" << formatNumber(con) << "\n";
    std::cout << "Energy: " << formatNumber(energyShares) << " shares @ $" << formatNumber(ind.energy.price) <<
    " = $" << formatNumber(ene) << "\n";
    std::cout << "Services: " << formatNumber(servicesShares) << " shares @ $" << formatNumber(ind.services.price) <<
    " = $" << formatNumber(ser) << "\n";
    std::cout << "Bond Principal: $" << formatNumber(bondPrincipal) << "\n";
    std::cout << "Bond Interest Rate: " << formatNumber(bondIntRate * 100) << "%\n";
    std::cout << "Index: " << formatNumber(indexShares) << " shares @ $" << formatNumber(ind.indexPrice) <<
    " = $" << formatNumber(idx) << "\n";
    std::cout << "Total Portfolio Value: $" << formatNumber(cash + agr + man + con + ene + ser + idx + bondPrincipal) << "\n";
    std::cout << "--------------------------------" << std::endl;
}

void Investor::buy(std::string target, double amount) {
    if (amount > cash) {
        std::cout << "Not enough cash." << std::endl;
        return;
    }

    if (target == "AGR") {
        agricultureShares += amount / ind.agriculture.price;
    } else if (target == "MAN") {
        manufacturingShares += amount / ind.manufacturing.price;
    } else if (target == "CON") {
        constructionShares +=  amount / ind.construction.price;
    } else if (target == "ENE") {
        energyShares +=  amount / ind.energy.price;
    } else if (target == "SER") {
        servicesShares +=  amount / ind.services.price;
    } else if (target == "IDX") {
        indexShares +=  amount / ind.indexPrice;
    } else if (target == "BND") {
        bondPrincipal += amount;
    } else {
        std::cout << "Unknown" << std::endl;
        return;
    }

    cash -= amount;
}

void Investor::sell(std::string target, double amount) {
    if (target == "AGR") {
        if (agricultureShares * ind.agriculture.price < amount) {
            std::cout << "Not enough shares." << std::endl;
            return;
        }
        agricultureShares -= amount / ind.agriculture.price;
    } else if (target == "MAN") {
        if (manufacturingShares * ind.manufacturing.price < amount) {
            std::cout << "Not enough shares." << std::endl;
            return;
        }
        manufacturingShares -= amount / ind.manufacturing.price;
    } else if (target == "CON") {
        if (constructionShares * ind.construction.price < amount) {
            std::cout << "Not enough shares." << std::endl;
            return;
        }
       constructionShares -= amount / ind.construction.price;
    } else if (target == "ENE") {
        if (energyShares * ind.energy.price < amount) {
            std::cout << "Not enough shares." << std::endl;
            return;
        }
        energyShares -= amount / ind.energy.price;
    } else if (target == "SER") {
        if (servicesShares * ind.services.price < amount) {
            std::cout << "Not enough shares." << std::endl;
            return;
        }
        servicesShares-= amount / ind.services.price;
    } else if (target == "BND") {
        if (amount > bondPrincipal) {
            std::cout << "Not enough bond principal." << std::endl;
            return;
        }
        bondPrincipal -= amount;
    } else {
        std::cout << "Unknown" << std::endl;
        return;
    }

    cash += amount;
}

double Investor::totalValue() {
    return cash
        + agricultureShares * ind.agriculture.price
        + manufacturingShares * ind.manufacturing.price
        + constructionShares * ind.construction.price
        + energyShares * ind.energy.price
        + servicesShares * ind.services.price
        + indexShares * ind.indexPrice
        + bondPrincipal;
}