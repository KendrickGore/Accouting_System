#include "CategoryManager.h"
#include <algorithm>

CategoryManager::CategoryManager() {
    categories.emplace_back("Food");
    categories.emplace_back("Transport");
    categories.emplace_back("Salary");
    categories.emplace_back("Investment");
    categories.emplace_back("Entertainment");
}

void CategoryManager::addCategory(const std::string &name) {
    if (!hasCategory(name)) {
        categories.emplace_back(name);
    }
}

void CategoryManager::removeCategory(const std::string &name) {
    categories.erase(
        std::remove_if(categories.begin(), categories.end(),
                       [&name](const Category &c) { return c.getName() == name; }),
        categories.end());
}

bool CategoryManager::hasCategory(const std::string &name) const {
    for (const auto &c : categories) {
        if (c.getName() == name) return true;
    }
    return false;
}

const std::vector<Category> &CategoryManager::getCategories() const {
    return categories;
}
