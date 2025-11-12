#ifndef SUBSCRIPTION_MANAGER_H
#define SUBSCRIPTION_MANAGER_H

#include <vector>
#include "Subscription.h"

class SubscriptionManager {
public:
    SubscriptionManager();

    int addSubscription(const std::string &name,
                        double amount,
                        const Date &nextDate);

    // 用于从 JSON 中恢复指定 id 的订阅
    void addSubscriptionWithId(int id,
                               const std::string &name,
                               double amount,
                               const Date &nextDate);

    void removeSubscription(int id);
    void listSubscriptions() const;
    void checkReminders(const Date &today, int daysBefore = 3) const;

    const std::vector<Subscription> &getAll() const;

private:
    std::vector<Subscription> subscriptions;
    int nextId;
};

#endif  // SUBSCRIPTION_MANAGER_H
