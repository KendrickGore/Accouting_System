#include "Date.h"

#include <cstdio>
#include <ctime>
#include <chrono>

Date::Date() : year(2025), month(11), day(12) {}

Date::Date(int y, int m, int d) : year(y), month(m), day(d) {}

Date Date::today() {
    std::time_t now = std::time(nullptr);

    std::tm localTime{};
#if defined(_WIN32) || defined(_WIN64)
    // Windows: localtime_s(dest, source)
    localtime_s(&localTime, &now);
#else
    // POSIX: localtime_r(source, dest)
    localtime_r(&now, &localTime);
#endif

    int y = localTime.tm_year + 1900;
    int m = localTime.tm_mon + 1;
    int d = localTime.tm_mday;
    return Date(y, m, d);
}

Date Date::parse(const std::string &text) {
    int y = 0, m = 0, d = 0;
    std::sscanf(text.c_str(), "%d-%d-%d", &y, &m, &d);
    return Date(y, m, d);
}

std::string Date::toString() const {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d", year, month, day);
    return std::string(buf);
}

bool Date::operator==(const Date &other) const {
    return year == other.year && month == other.month && day == other.day;
}

bool Date::operator<(const Date &other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}
