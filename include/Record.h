#ifndef RECORD_H
#define RECORD_H

#include <string>
#include "Date.h"

enum class RecordType {
    INCOME,
    EXPENSE
};

class Record {
public:
    Record();
    Record(int id,
           RecordType type,
           double amount,
           const std::string &category,
           const Date &date,
           const std::string &note);

    int getId() const;
    RecordType getType() const;
    double getAmount() const;
    const std::string &getCategory() const;
    const Date &getDate() const;
    const std::string &getNote() const;

    void setType(RecordType t);
    void setAmount(double a);
    void setCategory(const std::string &c);
    void setDate(const Date &d);
    void setNote(const std::string &n);

private:
    int id;
    RecordType type;
    double amount;
    std::string category;
    Date date;
    std::string note;
};

#endif  // RECORD_H
