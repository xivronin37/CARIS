
#include "init.h"
#include "economy.h"
#include "gov.h"
#include "industry.h"
#include "invest.h"

Investor::Investor(Economy& economy, Government& government, Industries& industries) : econ(economy), gov(government), ind(industries) {}

void Investor::InvestorInit() {
    cash = 10000.0;
}

void Investor::update() {

}