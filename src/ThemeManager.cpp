#include "ThemeManager.h"
#include <iostream>

ThemeManager::ThemeManager() : current(0) {
    themes.push_back("Light");
    themes.push_back("BusinessBlue");
    themes.push_back("WarmPink");
}

void ThemeManager::listThemes() const {
    std::cout << "Available themes:\n";
    for (size_t i = 0; i < themes.size(); ++i) {
        std::cout << i << ": " << themes[i] << "\n";
    }
}

void ThemeManager::setTheme(int index) {
    if (index >= 0 && index < static_cast<int>(themes.size())) {
        current = index;
    }
}

std::string ThemeManager::currentTheme() const {
    return themes[current];
}

void ThemeManager::setThemeByName(const std::string &name) {
    for (size_t i = 0; i < themes.size(); ++i) {
        if (themes[i] == name) {
            current = static_cast<int>(i);
            return;
        }
    }
}
