#include "AccountingSystem.h"
#include <iostream>
#include <cstdio>
#include <limits>

AccountingSystem::AccountingSystem()
    : storage("data.json") {}

void AccountingSystem::initialize() {
    storage.load(user,
        categoryManager,
        subscriptionManager,
        budgetReminder,
        themeManager,
        birthdayReminder);

    std::cout << "==== Accounting System Initialize ====\n";

    if (user.getName().empty() || user.getName() == "Guest") {
        std::cout << "Input your name: ";
        std::string name;
        std::getline(std::cin, name);
        if (!name.empty()) {
            user.setName(name);
        }
        else {
            user.setName("Guest");
        }
    }

    if (!birthdayReminder.isSet()) {
        while (true) {
            std::cout << "Please enter your birthday (YYYY-MM-DD): ";
            std::string s;
            std::getline(std::cin, s);
            if (!s.empty()) {
                Date d = Date::parse(s);
                birthdayReminder.setBirthday(d, user.getName());
                break;
            }
        }
        storage.save(user,
            categoryManager,
            subscriptionManager,
            budgetReminder,
            themeManager,
            birthdayReminder);
    }

    std::cout << "Hello, " << user.getName() << "!\n";
}

void AccountingSystem::mainMenu() {
    int choice = -1;
    while (choice != 0) {
        checkAllReminders();
        std::cout << "\n===== Main Menu (Theme: " << themeManager.currentTheme() << ") =====\n";
        std::cout << "1. Add Record\n";
        std::cout << "2. List Records\n";
        std::cout << "3. Edit Record\n";
        std::cout << "4. Statistics & Charts\n";
        std::cout << "5. Subscription Management\n";
        std::cout << "6. Budget Settings\n";
        std::cout << "7. Birthday\n";
        std::cout << "8. Theme\n";
        std::cout << "9. Finance Advice\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: menuAddRecord(); break;
            case 2: menuListRecords(); break;
            case 3: menuEditRecord(); break;
            case 4: menuStatistics(); break;
            case 5: menuSubscriptions(); break;
            case 6: menuBudget(); break;
            case 7: menuBirthday(); break;
            case 8: menuTheme(); break;
            case 9: menuFinanceAdvice(); break;
            case 0:
                std::cout << "Bye.\n";
                storage.save(user,
                             categoryManager,
                             subscriptionManager,
                             budgetReminder,
                             themeManager,
                             birthdayReminder);
                break;
            default:
                std::cout << "Invalid.\n";
                break;
        }
    }
}

void AccountingSystem::menuAddRecord() {
    std::cout << "--- Add Record ---\n";
    std::cout << "Type (1=Income, 2=Expense): ";
    int t;
    std::cin >> t;
    std::cout << "Amount: ";
    double amount;
    std::cin >> amount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Date (YYYY-MM-DD): ";
    std::string dateStr;
    std::getline(std::cin, dateStr);
    Date d = Date::parse(dateStr);

    std::cout << "Category (existing or new): ";
    std::string cat;
    std::getline(std::cin, cat);
    if (!categoryManager.hasCategory(cat)) {
        categoryManager.addCategory(cat);
    }

    std::cout << "Note: ";
    std::string note;
    std::getline(std::cin, note);

    int id = user.nextRecordId();
    RecordType rt = (t == 1) ? RecordType::INCOME : RecordType::EXPENSE;
    Record r(id, rt, amount, cat, d, note);
    user.getRecords().push_back(r);

    if (rt == RecordType::EXPENSE) {
        budgetReminder.updateCurrentMonthExpense(amount);
    }

    std::cout << "Record added, id=" << id << "\n";
    budgetReminder.checkAndNotify();

    // 顺手保存一次
    storage.save(user,
                 categoryManager,
                 subscriptionManager,
                 budgetReminder,
                 themeManager,
                 birthdayReminder);
}

void AccountingSystem::menuListRecords() {
    std::cout << "--- Records ---\n";

    // // BUG#1：可能的空指针解引用（类别 2：空指针引用）
    // const Record* debugPtr = nullptr;
    // if (!user.getRecords().empty() && user.getRecords()[0].getAmount() > 10000) {
    //     debugPtr = &user.getRecords()[0];
    // }
    // // 如果上面的条件不成立，debugPtr 仍然是 nullptr，这里会解引用空指针
    // std::cout << "[Debug] first amount: " << debugPtr->getAmount() << "\n";

    for (const auto &r : user.getRecords()) {
        std::cout << "#" << r.getId()
                  << " [" << (r.getType() == RecordType::INCOME ? "IN" : "OUT") << "] "
                  << r.getAmount()
                  << " " << r.getCategory()
                  << " " << r.getDate().toString()
                  << " Note: " << r.getNote()
                  << "\n";
    }
}

void AccountingSystem::menuEditRecord() {
    std::cout << "Input record id to edit: ";
    int id;
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    for (auto &r : user.getRecords()) {
        if (r.getId() == id) {
            std::cout << "Editing record #" << id << "\n";
            std::cout << "New amount (current " << r.getAmount() << "): ";
            double a;
            std::cin >> a;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            r.setAmount(a);
            std::cout << "New note: ";
            std::string note;
            std::getline(std::cin, note);
            r.setNote(note);
            std::cout << "Updated.\n";

            storage.save(user,
                         categoryManager,
                         subscriptionManager,
                         budgetReminder,
                         themeManager,
                         birthdayReminder);
            return;
        }
    }
    std::cout << "Not found.\n";
}

void AccountingSystem::menuStatistics() {
    auto stats = statisticsManager.calculate(user.getRecords());
    std::cout << "Total Income: " << stats.totalIncome << "\n";
    std::cout << "Total Expense: " << stats.totalExpense << "\n";

    // // BUG#2：数组越界（类别 3：数组越界）
    // double debugHistory[12] = { 0.0 };
    // for (int i = 0; i <= 12; ++i) {   // i == 12 时写越界
    //     debugHistory[i] = static_cast<double>(i);
    // }

    std::cout << "1. Show category pie\n";
    std::cout << "2. Show monthly bar\n";
    std::cout << "Other. Back\n";
    int c;
    std::cin >> c;
    if (c == 1) {
        chartGenerator.generateCategoryPie(user.getRecords());
    } else if (c == 2) {
        chartGenerator.generateMonthlyBar(user.getRecords());
    }
}

void AccountingSystem::menuSubscriptions() {
    int c = -1;
    while (c != 0) {
        std::cout << "--- Subscriptions ---\n";
        std::cout << "1. List\n";
        std::cout << "2. Add\n";
        std::cout << "3. Remove\n";
        std::cout << "0. Back\n";
        std::cin >> c;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (c == 1) {
            subscriptionManager.listSubscriptions();
        } else if (c == 2) {
            std::string name, dateStr;
            double amount;
            std::cout << "Name: ";
            std::getline(std::cin, name);
            std::cout << "Amount: ";
            std::cin >> amount;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Next date (YYYY-MM-DD): ";
            std::getline(std::cin, dateStr);
            Date d = Date::parse(dateStr);
            subscriptionManager.addSubscription(name, amount, d);

            storage.save(user,
                         categoryManager,
                         subscriptionManager,
                         budgetReminder,
                         themeManager,
                         birthdayReminder);
        } else if (c == 3) {
            std::cout << "ID: ";
            int id;
            std::cin >> id;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            subscriptionManager.removeSubscription(id);

            storage.save(user,
                         categoryManager,
                         subscriptionManager,
                         budgetReminder,
                         themeManager,
                         birthdayReminder);
        }
    }
}

void AccountingSystem::menuBudget() {
    std::cout << "Current budget: " << budgetReminder.getMonthlyBudget() << "\n";
    std::cout << "Set new budget: ";
    double b;
    std::cin >> b;
    budgetReminder.setMonthlyBudget(b);
    std::cout << "Budget set.\n";

    storage.save(user,
                 categoryManager,
                 subscriptionManager,
                 budgetReminder,
                 themeManager,
                 birthdayReminder);
}

void AccountingSystem::menuBirthday() {
    std::cout << "Set birthday (YYYY-MM-DD): ";
    std::string s;
    std::cin >> s;
    Date d = Date::parse(s);
    birthdayReminder.setBirthday(d, user.getName());
    std::cout << "Birthday set.\n";

    storage.save(user,
                 categoryManager,
                 subscriptionManager,
                 budgetReminder,
                 themeManager,
                 birthdayReminder);
}

void AccountingSystem::menuTheme() {
    themeManager.listThemes();
    std::cout << "Choose index: ";
    int idx;
    std::cin >> idx;
    themeManager.setTheme(idx);

    storage.save(user,
                 categoryManager,
                 subscriptionManager,
                 budgetReminder,
                 themeManager,
                 birthdayReminder);
}

void AccountingSystem::menuFinanceAdvice() {
    std::string plan = financeAdvisor.suggestPlan(user.getRecords());
    std::cout << plan << "\n";

    // // BUG#3：使用用户输入作为 printf 的格式串（类别 6：安全漏洞）
    // std::string logTag;
    // std::cout << "Input log tag: ";
    // std::getline(std::cin, logTag);
    // std::printf(logTag.c_str());  // 直接把用户输入作为格式串
}

void AccountingSystem::checkAllReminders() {
    Date today = Date::today();
    birthdayReminder.checkBirthday(today);
    subscriptionManager.checkReminders(today);
    budgetReminder.checkAndNotify();
}