#ifndef ACCOUNTING_SYSTEM_H
#define ACCOUNTING_SYSTEM_H

#include "User.h"
#include "CategoryManager.h"
#include "StatisticsManager.h"
#include "ChartGenerator.h"
#include "SubscriptionManager.h"
#include "BudgetReminder.h"
#include "BirthdayReminder.h"
#include "FinanceAdvisor.h"
#include "ThemeManager.h"
#include "JsonStorage.h"

class AccountingSystem {
public:
    AccountingSystem();

    void initialize();
    void mainMenu();

private:
    User user;
    CategoryManager categoryManager;
    StatisticsManager statisticsManager;
    ChartGenerator chartGenerator;
    SubscriptionManager subscriptionManager;
    BudgetReminder budgetReminder;
    BirthdayReminder birthdayReminder;
    FinanceAdvisor financeAdvisor;
    ThemeManager themeManager;

    JsonStorage storage;

    void menuAddRecord();
    void menuListRecords();
    void menuEditRecord();
    void menuStatistics();
    void menuSubscriptions();
    void menuBudget();
    void menuBirthday();
    void menuTheme();
    void menuFinanceAdvice();
    void checkAllReminders();
};

#endif  // ACCOUNTING_SYSTEM_H
