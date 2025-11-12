#include "Record.h"

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
void Record::setAmount(double a) { amount = a; }
void Record::setCategory(const std::string &c) { category = c; }
void Record::setDate(const Date &d) { date = d; }
void Record::setNote(const std::string &n) { note = n; }
