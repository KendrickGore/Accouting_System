#ifndef STATISTICS_MANAGER_H
#define STATISTICS_MANAGER_H

#include <vector>
#include <string>
#include "Record.h"

struct StatsResult {
    double totalIncome;
    double totalExpense;
};

class StatisticsManager {
public:
    StatisticsManager() = default;

    StatsResult calculate(const std::vector<Record> &records) const;
    double calculateByCategory(const std::vector<Record> &records,
                               const std::string &category,
                               RecordType type) const;
};

#endif  // STATISTICS_MANAGER_H
