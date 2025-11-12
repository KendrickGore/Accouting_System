#ifndef BIRTHDAY_REMINDER_H
#define BIRTHDAY_REMINDER_H

#include "Date.h"
#include <string>

class BirthdayReminder {
public:
    BirthdayReminder();

    void setBirthday(const Date& d, const std::string& name);
    void checkBirthday(const Date& today) const;

    Date getBirthday() const;
    std::string getUsername() const;

    // 新增：判断有没有真正设置过生日
    bool isSet() const;

private:
    Date birthday;
    std::string username;
    bool hasBirthday;   // 新增
};

#endif  // BIRTHDAY_REMINDER_H
