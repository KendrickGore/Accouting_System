#include <gtest/gtest.h>
#include <cstdio>
#include <string>
#include <filesystem>
#include <fstream>

#include "JsonStorage.h"
#include "User.h"
#include "CategoryManager.h"
#include "SubscriptionManager.h"
#include "BudgetReminder.h"
#include "ThemeManager.h"
#include "BirthdayReminder.h"
#include "Date.h"
#include "Record.h"

static std::string TempPath(const std::string& name) {
    namespace fs = std::filesystem;
    return (fs::temp_directory_path() / name).string();
}

// Save->Load 后，核心状态（用户名/记录/分类/预算/主题/生日）应能复原
TEST(Integration_Storage, SaveLoad_RoundTripPreservesCoreState) {
    std::string path = TempPath("accountingbook_it_roundtrip_core.json");
    std::remove(path.c_str());

    User user;
    user.setName("Kendrick");

    CategoryManager cm;
    cm.addCategory("__coffee__");

    BudgetReminder budget;
    budget.setMonthlyBudget(100.0);
    budget.updateCurrentMonthExpense(12.5);

    ThemeManager theme;
    theme.setTheme(2); // WarmPink

    BirthdayReminder birthday;
    birthday.setBirthday(Date::parse("2000-01-01"), user.getName());

    SubscriptionManager subs;

    user.getRecords().emplace_back(user.nextRecordId(), RecordType::EXPENSE, 12.5, "__coffee__",
                                   Date::parse("2025-12-31"), "latte");
    user.getRecords().emplace_back(user.nextRecordId(), RecordType::INCOME, 999.0, "Salary",
                                   Date::parse("2025-12-01"), "payday");

    JsonStorage storage(path);
    ASSERT_TRUE(storage.save(user, cm, subs, budget, theme, birthday));

    User user2;
    CategoryManager cm2;
    SubscriptionManager subs2;
    BudgetReminder budget2;
    ThemeManager theme2;
    BirthdayReminder birthday2;

    JsonStorage storage2(path);
    ASSERT_TRUE(storage2.load(user2, cm2, subs2, budget2, theme2, birthday2));

    EXPECT_EQ(user2.getName(), "Kendrick");
    EXPECT_TRUE(cm2.hasCategory("__coffee__"));
    ASSERT_EQ(user2.getRecords().size(), 2u);
    EXPECT_DOUBLE_EQ(budget2.getMonthlyBudget(), 100.0);
    EXPECT_DOUBLE_EQ(budget2.getCurrentMonthExpense(), 12.5);
    EXPECT_EQ(theme2.currentTheme(), "WarmPink");
    EXPECT_TRUE(birthday2.isSet());
    EXPECT_EQ(birthday2.getBirthday().toString(), Date::parse("2000-01-01").toString());

    std::remove(path.c_str());
}

// RoundTrip 后 recordIdCounter 应恢复到 maxId+1（下一次 nextRecordId 应正确）
TEST(Integration_Storage, SaveLoad_RoundTripRestoresNextRecordId) {
    std::string path = TempPath("accountingbook_it_roundtrip_id.json");
    std::remove(path.c_str());

    User user;
    user.setName("K");
    CategoryManager cm;
    SubscriptionManager subs;
    BudgetReminder budget;
    ThemeManager theme;
    BirthdayReminder birthday;

    // 人为指定非连续 id（模拟外部导入/旧数据）
    user.getRecords().emplace_back(7, RecordType::EXPENSE, 1.0, "Food", Date::parse("2025-01-01"), "");
    user.getRecords().emplace_back(42, RecordType::EXPENSE, 2.0, "Food", Date::parse("2025-01-02"), "");

    JsonStorage storage(path);
    ASSERT_TRUE(storage.save(user, cm, subs, budget, theme, birthday));

    User user2;
    CategoryManager cm2;
    SubscriptionManager subs2;
    BudgetReminder budget2;
    ThemeManager theme2;
    BirthdayReminder birthday2;

    JsonStorage storage2(path);
    ASSERT_TRUE(storage2.load(user2, cm2, subs2, budget2, theme2, birthday2));

    int next = user2.nextRecordId();
    EXPECT_EQ(next, 43);

    std::remove(path.c_str());
}

// 缺失文件 load 应返回 false，但不崩溃
TEST(Integration_Storage, LoadMissingFile_ReturnsFalse) {
    std::string path = TempPath("accountingbook_it_missing.json");
    std::remove(path.c_str());

    User user;
    CategoryManager cm;
    SubscriptionManager subs;
    BudgetReminder budget;
    ThemeManager theme;
    BirthdayReminder birthday;

    JsonStorage storage(path);
    EXPECT_FALSE(storage.load(user, cm, subs, budget, theme, birthday));
}

// 非法 JSON 内容 load 应返回 false，但不崩溃
TEST(Integration_Storage, LoadInvalidJson_ReturnsFalse) {
    std::string path = TempPath("accountingbook_it_badjson.json");
    std::remove(path.c_str());

    {
        std::ofstream out(path);
        out << "{ this is not valid json ...";
    }

    User user;
    CategoryManager cm;
    SubscriptionManager subs;
    BudgetReminder budget;
    ThemeManager theme;
    BirthdayReminder birthday;

    JsonStorage storage(path);
    EXPECT_FALSE(storage.load(user, cm, subs, budget, theme, birthday));

    std::remove(path.c_str());
}

// 同一路径二次 save 应覆盖更新后的状态（load 看到新值）
TEST(Integration_Storage, SaveTwice_OverwritesState) {
    std::string path = TempPath("accountingbook_it_overwrite.json");
    std::remove(path.c_str());

    User user;
    user.setName("A");
    CategoryManager cm;
    SubscriptionManager subs;
    BudgetReminder budget;
    ThemeManager theme;
    BirthdayReminder birthday;

    JsonStorage storage(path);
    ASSERT_TRUE(storage.save(user, cm, subs, budget, theme, birthday));

    user.setName("B");
    budget.setMonthlyBudget(123.0);
    ASSERT_TRUE(storage.save(user, cm, subs, budget, theme, birthday));

    User user2;
    CategoryManager cm2;
    SubscriptionManager subs2;
    BudgetReminder budget2;
    ThemeManager theme2;
    BirthdayReminder birthday2;

    JsonStorage storage2(path);
    ASSERT_TRUE(storage2.load(user2, cm2, subs2, budget2, theme2, birthday2));

    EXPECT_EQ(user2.getName(), "B");
    EXPECT_DOUBLE_EQ(budget2.getMonthlyBudget(), 123.0);

    std::remove(path.c_str());
}
