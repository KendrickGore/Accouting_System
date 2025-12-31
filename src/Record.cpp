#include "Record.h"
#include <stdexcept>
#include <algorithm>

Record::Record()
    : id(-1), type(RecordType::EXPENSE), amount(0.0),
      category("Uncategorized"), date(Date::today()), note("") {}

Record::Record(int id_,
               RecordType type_,
               double amount_,
               const std::string &category_,
               const Date &date_,
               const std::string &note_)
    : id(id_),
      type(type_),
      amount(amount_),
      category(category_),
      date(date_),
      note(note_) {}

int Record::getId() const { return id; }
RecordType Record::getType() const { return type; }
double Record::getAmount() const { return amount; }
const std::string &Record::getCategory() const { return category; }
const Date &Record::getDate() const { return date; }
const std::string &Record::getNote() const { return note; }

void Record::setType(RecordType t) { type = t; }
void Record::setAmount(double a) {
    if (a <= 0.0 || a > 1e6) {  // 假设金额上限为 1,000,000
        throw std::invalid_argument("Amount must be greater than 0 and less than or equal to 1,000,000.");
    }
    amount = a;
}

void Record::setCategory(const std::string &c) {
    std::string trimmed = c;
    // 去除首尾空格
    trimmed.erase(0, trimmed.find_first_not_of(" \t"));
    trimmed.erase(trimmed.find_last_not_of(" \t") + 1);

    if (trimmed.empty()) {
        throw std::invalid_argument("Category name cannot be empty.");
    }
    if (trimmed.size() > 50) {  // 假设分类名长度上限为 50
        throw std::invalid_argument("Category name exceeds the maximum limit of 50 characters.");
    }
    category = trimmed;
}
void Record::setDate(const Date &d) { date = d; }
void Record::setNote(const std::string &n) {
    if (n.size() > 256) {  // 假设备注长度上限为 256
        throw std::invalid_argument("Note length exceeds the maximum limit of 256 characters.");
    }
    note = n;
}
