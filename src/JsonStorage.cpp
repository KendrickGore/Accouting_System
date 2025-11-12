#include "JsonStorage.h"
#include "json.hpp"

#include <fstream>
#include <iostream>

using json = nlohmann::json;

JsonStorage::JsonStorage(const std::string &filePath) : file(filePath) {}

bool JsonStorage::load(User &user,
                       CategoryManager &categoryManager,
                       SubscriptionManager &subscriptionManager,
                       BudgetReminder &budgetReminder,
                       ThemeManager &themeManager,
                       BirthdayReminder &birthdayReminder) {
    std::ifstream in(file);
    if (!in.is_open()) {
        std::cerr << "[JsonStorage] file not found, start with defaults.\n";
        return false;
    }

    json j;
    try {
        in >> j;
    } catch (std::exception &e) {
        std::cerr << "[JsonStorage] parse error: " << e.what() << "\n";
        return false;
    }

    // user
    if (j.contains("user")) {
        auto ju = j["user"];
        if (ju.contains("name")) {
            user.setName(ju["name"].get<std::string>());
        }
        if (ju.contains("theme")) {
            themeManager.setThemeByName(ju["theme"].get<std::string>());
        }
        if (ju.contains("birthday")) {
            bool set = ju.value("birthday_set", false);
            if (set) {
                Date bd = Date::parse(ju["birthday"].get<std::string>());
                birthdayReminder.setBirthday(bd, user.getName());
            }
        }
    }

    // categories
    if (j.contains("categories")) {
        for (auto &cname : j["categories"]) {
            categoryManager.addCategory(cname.get<std::string>());
        }
    }

    // records
    int maxId = 0;
    if (j.contains("records")) {
        for (auto &rj : j["records"]) {
            int id = rj.value("id", -1);
            std::string typeStr = rj.value("type", "EXPENSE");
            double amount = rj.value("amount", 0.0);
            std::string category = rj.value("category", "Uncategorized");
            std::string dateStr = rj.value("date", "1970-01-01");
            std::string note = rj.value("note", "");
            RecordType rt = (typeStr == "INCOME") ? RecordType::INCOME : RecordType::EXPENSE;
            Record rec(id, rt, amount, category, Date::parse(dateStr), note);
            user.getRecords().push_back(rec);
            if (id > maxId) maxId = id;
        }
    }
    user.setRecordIdCounter(maxId + 1);

    // subscriptions
    if (j.contains("subscriptions")) {
        for (auto &sj : j["subscriptions"]) {
            int id = sj.value("id", -1);
            std::string name = sj.value("name", "");
            double amount = sj.value("amount", 0.0);
            std::string nd = sj.value("next_date", "1970-01-01");
            subscriptionManager.addSubscriptionWithId(id, name, amount, Date::parse(nd));
        }
    }

    // budget
    if (j.contains("budget")) {
        auto jb = j["budget"];
        if (jb.contains("monthly")) {
            budgetReminder.setMonthlyBudget(jb["monthly"].get<double>());
        }
        if (jb.contains("current_month_expense")) {
            budgetReminder.resetCurrentMonthExpense();
            budgetReminder.updateCurrentMonthExpense(
                jb["current_month_expense"].get<double>());
        }
    }

    return true;
}

bool JsonStorage::save(const User &user,
                       const CategoryManager &categoryManager,
                       const SubscriptionManager &subscriptionManager,
                       const BudgetReminder &budgetReminder,
                       const ThemeManager &themeManager,
                       const BirthdayReminder &birthdayReminder) {
    json j;

    // user
    j["user"]["name"] = user.getName();
    j["user"]["theme"] = themeManager.currentTheme();
    j["user"]["birthday"] = birthdayReminder.getBirthday().toString();
    j["user"]["birthday_set"] = birthdayReminder.isSet();
    // categories
    j["categories"] = json::array();
    for (const auto &c : categoryManager.getCategories()) {
        j["categories"].push_back(c.getName());
    }

    // records
    j["records"] = json::array();
    for (const auto &r : user.getRecords()) {
        json jr;
        jr["id"] = r.getId();
        jr["type"] = (r.getType() == RecordType::INCOME) ? "INCOME" : "EXPENSE";
        jr["amount"] = r.getAmount();
        jr["category"] = r.getCategory();
        jr["date"] = r.getDate().toString();
        jr["note"] = r.getNote();
        j["records"].push_back(jr);
    }

    // subscriptions
    j["subscriptions"] = json::array();
    for (const auto &s : subscriptionManager.getAll()) {
        json js;
        js["id"] = s.getId();
        js["name"] = s.getName();
        js["amount"] = s.getAmount();
        js["next_date"] = s.getNextDate().toString();
        j["subscriptions"].push_back(js);
    }

    // budget
    j["budget"]["monthly"] = budgetReminder.getMonthlyBudget();
    j["budget"]["current_month_expense"] = budgetReminder.getCurrentMonthExpense();

    std::ofstream out(file);
    if (!out.is_open()) {
        std::cerr << "[JsonStorage] cannot write file.\n";
        return false;
    }
    out << j.dump(4);
    return true;
}
