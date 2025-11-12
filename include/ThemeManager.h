#ifndef THEME_MANAGER_H
#define THEME_MANAGER_H

#include <string>
#include <vector>

class ThemeManager {
public:
    ThemeManager();

    void listThemes() const;
    void setTheme(int index);
    std::string currentTheme() const;

    // 为 JSON 读取加的：根据名字设置
    void setThemeByName(const std::string &name);

private:
    std::vector<std::string> themes;
    int current;
};

#endif  // THEME_MANAGER_H
