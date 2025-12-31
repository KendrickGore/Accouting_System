#include "Category.h"
#include <algorithm>
#include <stdexcept>

Category::Category(const std::string &name_) : name(name_) {}

const std::string &Category::getName() const {
    return name;
}

void Category::setName(const std::string &n) {
    std::string trimmed = n;
    // 去除首尾空格
    trimmed.erase(0, trimmed.find_first_not_of(" \t"));
    trimmed.erase(trimmed.find_last_not_of(" \t") + 1);

    if (trimmed.empty()) {
        throw std::invalid_argument("Category name cannot be empty.");
    }
    name = trimmed;
}
