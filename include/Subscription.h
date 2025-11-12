#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <string>
#include "Date.h"

class Subscription {
public:
    Subscription();
    Subscription(int id,
                 const std::string &name,
                 double amount,
                 const Date &nextDate);

    int getId() const;
    const std::string &getName() const;
    double getAmount() const;
    const Date &getNextDate() const;

    void setName(const std::string &n);
    void setAmount(double a);
    void setNextDate(const Date &d);

private:
    int id;
    std::string name;
    double amount;
    Date nextDate;
};

#endif  // SUBSCRIPTION_H
