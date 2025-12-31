#include <gtest/gtest.h>
#include <string>
#include "Record.h"
#include "Date.h"

// 默认构造应给出约定默认值
TEST(RecordTest, DefaultConstructor_HasExpectedDefaults) {
    Record r;
    EXPECT_EQ(r.getId(), -1);
    EXPECT_EQ(r.getType(), RecordType::EXPENSE);
    EXPECT_DOUBLE_EQ(r.getAmount(), 0.0);
    EXPECT_EQ(r.getCategory(), "Uncategorized");
    EXPECT_TRUE(r.getNote().empty());
    EXPECT_EQ(r.getDate().toString(), Date::today().toString());
}

// 自定义构造后各字段应匹配
TEST(RecordTest, CustomConstructor_FieldsMatch) {
    Date d = Date::parse("2025-12-30");
    Record r(7, RecordType::INCOME, 88.5, "Salary", d, "bonus");
    EXPECT_EQ(r.getId(), 7);
    EXPECT_EQ(r.getType(), RecordType::INCOME);
    EXPECT_DOUBLE_EQ(r.getAmount(), 88.5);
    EXPECT_EQ(r.getCategory(), "Salary");
    EXPECT_EQ(r.getDate().toString(), d.toString());
    EXPECT_EQ(r.getNote(), "bonus");
}

// setType 应更新类型
TEST(RecordTest, SetType_Updates) {
    Record r;
    r.setType(RecordType::INCOME);
    EXPECT_EQ(r.getType(), RecordType::INCOME);
}

// setAmount 应更新金额
TEST(RecordTest, SetAmount_Updates) {
    Record r;
    r.setAmount(12.34);
    EXPECT_DOUBLE_EQ(r.getAmount(), 12.34);
}

// 金额不应允许为负
TEST(RecordTest, SetAmount_NegativeShouldBeRejected) {
    Record r;
    r.setAmount(-10.0);
    EXPECT_GE(r.getAmount(), 0.0);
}

// 金额不应允许为 0
TEST(RecordTest, SetAmount_ZeroShouldBeRejected) {
    Record r;
    r.setAmount(0.0);
    EXPECT_GT(r.getAmount(), 0.0);
}

// 超大金额应被限制在合理范围内
TEST(RecordTest, SetAmount_TooLargeShouldBeRejected) {
    Record r;
    r.setAmount(1e100);
    EXPECT_LT(r.getAmount(), 1e12); 
}

// setCategory 应更新类别
TEST(RecordTest, SetCategory_Updates) {
    Record r;
    r.setCategory("Food");
    EXPECT_EQ(r.getCategory(), "Food");
}

// 类别不应允许为空
TEST(RecordTest, SetCategory_EmptyShouldBeRejected) {
    Record r;
    r.setCategory("");
    EXPECT_FALSE(r.getCategory().empty());
}

// setCategory 应支持 Unicode
TEST(RecordTest, SetCategory_Unicode) {
    Record r;
    r.setCategory("餐饮");
    EXPECT_EQ(r.getCategory(), "餐饮");
}

// setDate 应更新日期
TEST(RecordTest, SetDate_Updates) {
    Record r;
    Date d = Date::parse("2024-01-01");
    r.setDate(d);
    EXPECT_EQ(r.getDate().toString(), d.toString());
}

// note 不应超过最大长度
TEST(RecordTest, SetNote_TooLongShouldBeRejected) {
    Record r;
    std::string longNote(5000, 'a');
    r.setNote(longNote);
    EXPECT_LE(r.getNote().size(), 200);
}
