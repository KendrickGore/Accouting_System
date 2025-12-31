#include <gtest/gtest.h>
#include <string>

#include "User.h"
#include "StatisticsManager.h"
#include "Date.h"
#include "Record.h"

// 多条收入/支出记录组合后，统计模块应能正确汇总总收入与总支出
TEST(Integration_Statistics, RecordsFromUser_CanBeAggregatedCorrectly) {
    User user;
    StatisticsManager stats;

    user.getRecords().emplace_back(user.nextRecordId(), RecordType::INCOME, 1000.0, "Salary",
                                   Date::parse("2025-01-01"), "payday");
    user.getRecords().emplace_back(user.nextRecordId(), RecordType::EXPENSE, 100.0, "Food",
                                   Date::parse("2025-01-02"), "lunch");
    user.getRecords().emplace_back(user.nextRecordId(), RecordType::EXPENSE, 50.0, "Transport",
                                   Date::parse("2025-01-03"), "metro");

    auto res = stats.calculate(user.getRecords());
    EXPECT_DOUBLE_EQ(res.totalIncome, 1000.0);
    EXPECT_DOUBLE_EQ(res.totalExpense, 150.0);
    EXPECT_DOUBLE_EQ(stats.calculateByCategory(user.getRecords(), "Food", RecordType::EXPENSE), 100.0);
}

// 空记录集合的统计应为 0
TEST(Integration_Statistics, EmptyRecords_ReturnZeroTotals) {
    User user;
    StatisticsManager stats;

    auto res = stats.calculate(user.getRecords());
    EXPECT_DOUBLE_EQ(res.totalIncome, 0.0);
    EXPECT_DOUBLE_EQ(res.totalExpense, 0.0);
}

// 查询不存在分类的按分类统计应为 0
TEST(Integration_Statistics, MissingCategory_ReturnsZero) {
    User user;
    StatisticsManager stats;

    user.getRecords().emplace_back(user.nextRecordId(), RecordType::EXPENSE, 10.0, "Food",
                                   Date::parse("2025-01-01"), "");
    EXPECT_DOUBLE_EQ(stats.calculateByCategory(user.getRecords(), "__no_such__", RecordType::EXPENSE), 0.0);
}

// 全为收入记录时，总支出应为 0
TEST(Integration_Statistics, AllIncome_TotalExpenseIsZero) {
    User user;
    StatisticsManager stats;

    user.getRecords().emplace_back(user.nextRecordId(), RecordType::INCOME, 10.0, "Salary",
                                   Date::parse("2025-01-01"), "");
    user.getRecords().emplace_back(user.nextRecordId(), RecordType::INCOME, 20.0, "Salary",
                                   Date::parse("2025-01-02"), "");

    auto res = stats.calculate(user.getRecords());
    EXPECT_DOUBLE_EQ(res.totalIncome, 30.0);
    EXPECT_DOUBLE_EQ(res.totalExpense, 0.0);
}

// 同一分类多条记录时，按分类汇总应为该分类同类型之和
TEST(Integration_Statistics, SameCategorySum_Works) {
    User user;
    StatisticsManager stats;

    user.getRecords().emplace_back(user.nextRecordId(), RecordType::EXPENSE, 5.0, "Food",
                                   Date::parse("2025-02-01"), "");
    user.getRecords().emplace_back(user.nextRecordId(), RecordType::EXPENSE, 7.5, "Food",
                                   Date::parse("2025-02-02"), "");
    user.getRecords().emplace_back(user.nextRecordId(), RecordType::EXPENSE, 2.0, "Transport",
                                   Date::parse("2025-02-03"), "");

    EXPECT_DOUBLE_EQ(stats.calculateByCategory(user.getRecords(), "Food", RecordType::EXPENSE), 12.5);
}
