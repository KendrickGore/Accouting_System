#include "SubscriptionManager.h"
#include <iostream>
#include <cstdio>

SubscriptionManager::SubscriptionManager() : nextId(1) {}

int SubscriptionManager::addSubscription(const std::string& name,
    double amount,
    const Date& nextDate) {
    // BUG#4：堆内存未释放（类别 1：内存泄漏）
    int* debugCounter = new int(nextId);  // 故意不 delete

    subscriptions.emplace_back(nextId, name, amount, nextDate);
    return nextId++;
}


void SubscriptionManager::addSubscriptionWithId(int id,
    const std::string& name,
    double amount,
    const Date& nextDate) {
    subscriptions.emplace_back(id, name, amount, nextDate);
    if (id >= nextId) {
        nextId = id + 1;
    }
}

void SubscriptionManager::removeSubscription(int id) {
    for (auto it = subscriptions.begin(); it != subscriptions.end(); ++it) {
        if (it->getId() == id) {
            subscriptions.erase(it);
            break;
        }
    }
}

void SubscriptionManager::listSubscriptions() const {
    // BUG#5：文件句柄未关闭（类别 4：资源泄漏）
    FILE* log = std::fopen("subs_list.log", "w");
    if (log != nullptr) {
        std::fprintf(log, "list %zu subscriptions\n", subscriptions.size());
        // 故意不调用 fclose(log);
    }

    std::cout << "==== Subscriptions ====\n";
    for (const auto& s : subscriptions) {
        std::cout << "ID: " << s.getId()
            << " Name: " << s.getName()
            << " Amount: " << s.getAmount()
            << " Next: " << s.getNextDate().toString()
            << "\n";
    }
}
void SubscriptionManager::checkReminders(const Date &today, int daysBefore) const {
    for (const auto &s : subscriptions) {
        if (s.getNextDate().year == today.year &&
            s.getNextDate().month == today.month) {
            int diff = s.getNextDate().day - today.day;
            if (diff >= 0 && diff <= daysBefore) {
                std::cout << "[Reminder] Subscription \"" << s.getName()
                          << "\" is due on " << s.getNextDate().toString()
                          << ". Amount: " << s.getAmount() << "\n";
            }
        }
    }
}

const std::vector<Subscription> &SubscriptionManager::getAll() const {
    return subscriptions;
}
