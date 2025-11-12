#ifndef CATEGORY_MANAGER_H
#define CATEGORY_MANAGER_H

#include <vector>
#include <string>
#include "Category.h"

class CategoryManager {
public:
    CategoryManager();

    void addCategory(const std::string &name);
    void removeCategory(const std::string &name);
    bool hasCategory(const std::string &name) const;
    const std::vector<Category> &getCategories() const;

private:
    std::vector<Category> categories;
};

#endif  // CATEGORY_MANAGER_H
