#include "Category.h"

Category::Category(const std::string &name_) : name(name_) {}

const std::string &Category::getName() const {
    return name;
}

void Category::setName(const std::string &n) {
    name = n;
}
