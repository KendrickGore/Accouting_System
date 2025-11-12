#include "Subscription.h"

Subscription::Subscription()
    : id(-1), name(""), amount(0.0), nextDate(Date::today()) {}

Subscription::Subscription(int id_,
                           const std::string &name_,
                           double amount_,
                           const Date &nextDate_)
    : id(id_), name(name_), amount(amount_), nextDate(nextDate_) {}

int Subscription::getId() const { return id; }
const std::string &Subscription::getName() const { return name; }
double Subscription::getAmount() const { return amount; }
const Date &Subscription::getNextDate() const { return nextDate; }

void Subscription::setName(const std::string &n) { name = n; }
void Subscription::setAmount(double a) { amount = a; }
void Subscription::setNextDate(const Date &d) { nextDate = d; }
