#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <vector>

#include "User.h"
#include "CategoryManager.h"
#include "BudgetReminder.h"
#include "Record.h"
#include "Date.h"

// 统计某个分类名出现次数（验证不会重复插入）
static int CountName(const std::vector<Category>& cats, const std::string& name) {
    return (int)std::count_if(cats.begin(), cats.end(),
                              [&](const Category& c){ return c.getName() == name; });
}

// 按 UI 的“添加记录”逻辑组装一次
static int AddRecordLikeUI(User& user,
                           CategoryManager& categoryManager,
                           BudgetReminder& budgetReminder,
                           RecordType rt,
                           double amount,
                           const std::string& category,
                           const Date& date,
                           const std::string& note) {
    if (!categoryManager.hasCategory(category)) {
        categoryManager.addCategory(category);
    }
    int id = user.nextRecordId();
    user.getRecords().emplace_back(id, rt, amount, category, date, note);
    if (rt == RecordType::EXPENSE) {
        budgetReminder.updateCurrentMonthExpense(amount);
    }
    return id;
}

// 新分类 + 支出记录应同时更新分类列表、记录列表与预算累计
TEST(Integration_RecordFlow, NewCategoryExpenseRecord_UpdatesAllModules) {
    User user;
    CategoryManager cm;
    BudgetReminder budget;

    const std::string cat = "__coffee__";
    ASSERT_FALSE(cm.hasCategory(cat));
    ASSERT_TRUE(user.getRecords().empty());
    ASSERT_DOUBLE_EQ(budget.getCurrentMonthExpense(), 0.0);

    Date d = Date::parse("2025-12-31");
    int id = AddRecordLikeUI(user, cm, budget, RecordType::EXPENSE, 12.5, cat, d, "latte");

    EXPECT_TRUE(cm.hasCategory(cat));
    ASSERT_EQ(user.getRecords().size(), 1u);
    EXPECT_EQ(user.getRecords()[0].getId(), id);
    EXPECT_EQ(user.getRecords()[0].getCategory(), cat);
    EXPECT_DOUBLE_EQ(budget.getCurrentMonthExpense(), 12.5);
}

// 已存在分类添加支出记录时，分类不应重复插入，预算应累加
TEST(Integration_RecordFlow, ExistingCategoryExpenseRecord_DoesNotDuplicateCategory) {
    User user;
    CategoryManager cm;
    BudgetReminder budget;

    const std::string cat = "Food"; // 默认分类
    ASSERT_TRUE(cm.hasCategory(cat));
    auto beforeSize = cm.getCategories().size();
    auto beforeCount = CountName(cm.getCategories(), cat);

    Date d = Date::parse("2025-02-01");
    AddRecordLikeUI(user, cm, budget, RecordType::EXPENSE, 20.0, cat, d, "meal");

    EXPECT_EQ(cm.getCategories().size(), beforeSize);
    EXPECT_EQ(CountName(cm.getCategories(), cat), beforeCount);
    EXPECT_DOUBLE_EQ(budget.getCurrentMonthExpense(), 20.0);
}

// 多次添加记录时，id 应自增且预算支出为所有 EXPENSE 之和
TEST(Integration_RecordFlow, MultipleRecords_IdMonotonicAndExpenseAccumulates) {
    User user;
    CategoryManager cm;
    BudgetReminder budget;

    Date d1 = Date::parse("2025-03-01");
    Date d2 = Date::parse("2025-03-02");
    Date d3 = Date::parse("2025-03-03");

    int id1 = AddRecordLikeUI(user, cm, budget, RecordType::EXPENSE, 10.0, "Food", d1, "a");
    int id2 = AddRecordLikeUI(user, cm, budget, RecordType::INCOME, 100.0, "Salary", d2, "b");
    int id3 = AddRecordLikeUI(user, cm, budget, RecordType::EXPENSE, 5.5, "Transport", d3, "c");

    ASSERT_EQ(user.getRecords().size(), 3u);
    EXPECT_LT(id1, id2);
    EXPECT_LT(id2, id3);
    EXPECT_DOUBLE_EQ(budget.getCurrentMonthExpense(), 15.5);
}

// 多次使用同一个“自定义分类”添加记录时，分类只应新增一次
TEST(Integration_RecordFlow, ReuseCustomCategory_CategoryAddedOnlyOnce) {
    User user;
    CategoryManager cm;
    BudgetReminder budget;

    const std::string cat = "__snack__";
    ASSERT_FALSE(cm.hasCategory(cat));

    auto beforeSize = cm.getCategories().size();
    Date d = Date::parse("2025-04-01");

    AddRecordLikeUI(user, cm, budget, RecordType::EXPENSE, 3.0, cat, d, "x");
    AddRecordLikeUI(user, cm, budget, RecordType::EXPENSE, 4.0, cat, d, "y");

    EXPECT_TRUE(cm.hasCategory(cat));
    EXPECT_EQ(cm.getCategories().size(), beforeSize + 1);
    EXPECT_EQ(CountName(cm.getCategories(), cat), 1);
    EXPECT_DOUBLE_EQ(budget.getCurrentMonthExpense(), 7.0);
}

// 预算重置后再添加支出记录，支出累计应从 0 重新开始累加
TEST(Integration_RecordFlow, ResetBudgetExpense_ThenAddExpense_Works) {
    User user;
    CategoryManager cm;
    BudgetReminder budget;

    Date d = Date::parse("2025-05-01");
    AddRecordLikeUI(user, cm, budget, RecordType::EXPENSE, 20.0, "Food", d, "a");
    ASSERT_DOUBLE_EQ(budget.getCurrentMonthExpense(), 20.0);

    budget.resetCurrentMonthExpense();
    ASSERT_DOUBLE_EQ(budget.getCurrentMonthExpense(), 0.0);

    AddRecordLikeUI(user, cm, budget, RecordType::EXPENSE, 7.0, "Food", d, "b");
    EXPECT_DOUBLE_EQ(budget.getCurrentMonthExpense(), 7.0);
}
