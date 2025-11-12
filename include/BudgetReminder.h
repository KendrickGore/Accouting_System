#ifndef BUDGET_REMINDER_H
#define BUDGET_REMINDER_H

class BudgetReminder {
public:
    BudgetReminder();

    void setMonthlyBudget(double budget);
    double getMonthlyBudget() const;

    void updateCurrentMonthExpense(double amount);
    void resetCurrentMonthExpense();
    double getCurrentMonthExpense() const;

    void checkAndNotify() const;

private:
    double monthlyBudget;
    double currentMonthExpense;
};

#endif  // BUDGET_REMINDER_H
