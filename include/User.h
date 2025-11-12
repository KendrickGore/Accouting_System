#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "Record.h"

class User {
public:
    User();

    void setName(const std::string &n);
    const std::string &getName() const;

    std::vector<Record> &getRecords();
    const std::vector<Record> &getRecords() const;

    int nextRecordId();
    void setRecordIdCounter(int v);

private:
    std::string name;
    std::vector<Record> records;
    int recordIdCounter;
};

#endif  // USER_H
