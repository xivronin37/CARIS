#pragma once

#include <deque>
#include "economy.h"
#include "gov.h"

enum class IndustryType {
    Agriculture, Manufacturing, Construction, Energy, Services
};

class Industry {
    private:
        double employmentLow;
        double employmentHigh;
        std::deque<double> employmentHistory;
        std::deque<double> outHistory;
    public:
        IndustryType type;
        double output;
        double outputShare;
        double productivity;
        double employment;
        double wage;
        double valuationMultiple = 0;
        bool calibrated = false;
        double price;

        double desiredEmployment(double wage);
        void update();
        double outputVal(double productivity, double employment);
};

class Industries {
    private:
        Economy& econ;
        Government& gov;
        int tickNumber = 0;
    public:
        double indexPrice;
        double previousTotal;
        bool hasPrevTotal = false;
        Industry agriculture;
        Industry manufacturing;
        Industry construction;
        Industry energy;
        Industry services;

        void industryInit();
        void update();
        double totalOutput();
        Industries(Economy& economy, Government& government);
};