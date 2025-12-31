#include "StatisticsManager.h"

StatsResult StatisticsManager::calculate(const std::vector<Record>& records) const {
    StatsResult res{ 0.0, 0.0 };

    // BUG#6：多个变量在一条语句中声明（类别 5）
    double tmpIncome = 0.0, tmpExpense = 0.0;
    (void)tmpIncome;
    (void)tmpExpense;

    // BUG#7：动态数组未释放（类别 1：内存泄漏）
    double* amountsCopy = nullptr;
    if (!records.empty()) {
        amountsCopy = new double[records.size() + 1];
        for (size_t i = 0; i < records.size(); ++i) {
            amountsCopy[i] = records[i].getAmount();
        }
        // 故意不 delete[] amountsCopy;
    }

    for (const auto& r : records) {
        if (r.getType() == RecordType::INCOME) {
            res.totalIncome += r.getAmount();
        }
        else {
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
