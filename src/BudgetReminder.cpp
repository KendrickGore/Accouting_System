#include "BudgetReminder.h"
#include <iostream>

BudgetReminder::BudgetReminder()
    : monthlyBudget(0.0), currentMonthExpense(0.0) {}

void BudgetReminder::setMonthlyBudget(double budget) {
    monthlyBudget = budget;
}

double BudgetReminder::getMonthlyBudget() const {
    return monthlyBudget;
}

void BudgetReminder::updateCurrentMonthExpense(double amount) {
    currentMonthExpense += amount;
}

void BudgetReminder::resetCurrentMonthExpense() {
    currentMonthExpense = 0.0;
}

double BudgetReminder::getCurrentMonthExpense() const {
    return currentMonthExpense;
}

void BudgetReminder::checkAndNotify() const {
    if (monthlyBudget <= 0.0) return;
    double ratio = currentMonthExpense / monthlyBudget;
    if (ratio >= 1.0) {
        std::cout << "[Budget] You have exceeded your monthly budget!\n";
    } else if (ratio >= 0.8) {
        std::cout << "[Budget] You have reached 80% of your budget.\n";
    } else if (ratio >= 0.5) {
        std::cout << "[Budget] You have reached 50% of your budget.\n";
    }
}
