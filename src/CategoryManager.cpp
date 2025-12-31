#include "CategoryManager.h"
#include <algorithm>
#include <stdexcept>

CategoryManager::CategoryManager() {
    categories.emplace_back("Food");
    categories.emplace_back("Transport");
    categories.emplace_back("Salary");
    categories.emplace_back("Investment");
    categories.emplace_back("Entertainment");
}

void CategoryManager::addCategory(const std::string &name) {
    if (!hasCategory(name)) {
        Category newCategory(name);
        categories.emplace_back(newCategory);
    }
}

void CategoryManager::removeCategory(const std::string &name) {
    // 禁止删除默认分类
    static const std::vector<std::string> defaultCategories = {
        "Food", "Transport", "Salary", "Investment", "Entertainment"};
    if (std::find(defaultCategories.begin(), defaultCategories.end(), name) != defaultCategories.end()) {
        throw std::invalid_argument("Cannot remove default category: " + name);
    }

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
