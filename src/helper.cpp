#include <string>
#include <sstream>
#include <iomanip>

#include "helper.h"


std::string formatNumber(double value) {
    const char* suffixes[] = {"", "K", "M", "B", "T", "Qa", "Qi"};
    int idx = 0;
    while (std::abs(value) >= 1000.0 && idx < 6) {
        value /= 1000.0;
        idx++;
    }
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << value << suffixes[idx];
    return out.str();
}