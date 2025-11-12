#include "StatisticsManager.h"

StatsResult StatisticsManager::calculate(const std::vector<Record> &records) const {
    StatsResult res{0.0, 0.0};
    for (const auto &r : records) {
        if (r.getType() == RecordType::INCOME) {
            res.totalIncome += r.getAmount();
        } else {
            res.totalExpense += r.getAmount();
        }
    }
    return res;
}

double StatisticsManager::calculateByCategory(const std::vector<Record> &records,
                                              const std::string &category,
                                              RecordType type) const {
    double sum = 0.0;
    for (const auto &r : records) {
        if (r.getType() == type && r.getCategory() == category) {
            sum += r.getAmount();
        }
    }
    return sum;
}
