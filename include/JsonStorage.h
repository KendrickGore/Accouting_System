#ifndef JSON_STORAGE_H
#define JSON_STORAGE_H

#include <string>
#include "User.h"
#include "CategoryManager.h"
#include "SubscriptionManager.h"
#include "BudgetReminder.h"
#include "ThemeManager.h"
#include "BirthdayReminder.h"

class JsonStorage {
public:
    explicit JsonStorage(const std::string &filePath);

    bool load(User &user,
              CategoryManager &categoryManager,
              SubscriptionManager &subscriptionManager,
              BudgetReminder &budgetReminder,
              ThemeManager &themeManager,
              BirthdayReminder &birthdayReminder);

    bool save(const User &user,
              const CategoryManager &categoryManager,
              const SubscriptionManager &subscriptionManager,
              const BudgetReminder &budgetReminder,
              const ThemeManager &themeManager,
              const BirthdayReminder &birthdayReminder);

private:
    std::string file;
};

#endif  // JSON_STORAGE_H
