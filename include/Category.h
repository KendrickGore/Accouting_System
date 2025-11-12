#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>

class Category {
public:
    Category() = default;
    explicit Category(const std::string &name);

    const std::string &getName() const;
    void setName(const std::string &n);

private:
    std::string name;
};

#endif  // CATEGORY_H
