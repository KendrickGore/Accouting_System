#ifndef CHART_GENERATOR_H
#define CHART_GENERATOR_H

#include <vector>
#include <string>
#include "Record.h"

class ChartGenerator {
public:
    ChartGenerator() = default;

    void generateCategoryPie(const std::vector<Record> &records) const;
    void generateMonthlyBar(const std::vector<Record> &records) const;
};

#endif  // CHART_GENERATOR_H
