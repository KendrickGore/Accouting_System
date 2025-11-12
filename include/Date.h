#ifndef DATE_H
#define DATE_H

#include <string>

class Date {
public:
    int year;
    int month;
    int day;

    Date();
    Date(int y, int m, int d);

    static Date today();
    static Date parse(const std::string &text);
    std::string toString() const;

    bool operator==(const Date &other) const;
    bool operator<(const Date &other) const;
};

#endif  // DATE_H