#include "SubscriptionManager.h"
#include <iostream>

SubscriptionManager::SubscriptionManager() : nextId(1) {}

int SubscriptionManager::addSubscription(const std::string &name,
                                         double amount,
                                         const Date &nextDate) {
    subscriptions.emplace_back(nextId, name, amount, nextDate);
    return nextId++;
}

void SubscriptionManager::addSubscriptionWithId(int id,
                                                const std::string &name,
                                                double amount,
                                                const Date &nextDate) {
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
    std::cout << "==== Subscriptions ====\n";
    for (const auto &s : subscriptions) {
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
