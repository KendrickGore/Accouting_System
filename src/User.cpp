#include "User.h"

User::User() : name("Guest"), recordIdCounter(1) {}

void User::setName(const std::string &n) {
    name = n;
}

const std::string &User::getName() const {
    return name;
}

std::vector<Record> &User::getRecords() {
    return records;
}

const std::vector<Record> &User::getRecords() const {
    return records;
}

int User::nextRecordId() {
    return recordIdCounter++;
}

void User::setRecordIdCounter(int v) {
    recordIdCounter = v;
}
