#include "ChartGenerator.h"
#include <iostream>
#include <map>

void ChartGenerator::generateCategoryPie(const std::vector<Record> &records) const {
    std::map<std::string, double> expenseByCat;
    for (const auto &r : records) {
        if (r.getType() == RecordType::EXPENSE) {
            expenseByCat[r.getCategory()] += r.getAmount();
        }
    }
    std::cout << "=== Expense Category Pie (text) ===\n";
    for (const auto &p : expenseByCat) {
        std::cout << p.first << ": " << p.second << "\n";
    }
}

void ChartGenerator::generateMonthlyBar(const std::vector<Record> &records) const {
    std::map<int, double> monthExpense;
    for (const auto &r : records) {
        if (r.getType() == RecordType::EXPENSE) {
            monthExpense[r.getDate().month] += r.getAmount();
        }
    }
    std::cout << "=== Monthly Expense Bar (text) ===\n";
    for (const auto &p : monthExpense) {
        std::cout << "Month " << p.first << ": " << p.second << "\n";
    }
}
