#include "BirthdayReminder.h"
#include <iostream>

BirthdayReminder::BirthdayReminder()
    : birthday(Date(1970, 1, 1)), username("User") {}

void BirthdayReminder::setBirthday(const Date& d, const std::string& name) {
    birthday = d;
    username = name;
    hasBirthday = true;
}

void BirthdayReminder::checkBirthday(const Date& today) const {
    if (!hasBirthday) {
        return;
    }
    if (today.month == birthday.month && today.day == birthday.day) {
        std::cout << "[Birthday] Happy Birthday, " << username << "!\n";
    }
}

Date BirthdayReminder::getBirthday() const {
    return birthday;
}

std::string BirthdayReminder::getUsername() const {
    return username;
}

bool BirthdayReminder::isSet() const {
    return hasBirthday;
}