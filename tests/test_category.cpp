#include <gtest/gtest.h>
#include <string>
#include "Category.h"

// 默认构造后名称应为空
TEST(CategoryTest, DefaultConstructor_NameIsEmpty) {
    Category c;
    EXPECT_TRUE(c.getName().empty());
}

// 带参构造后 getName 应返回同名
TEST(CategoryTest, ConstructWithName_GetNameMatches) {
    Category c("Food");
    EXPECT_EQ(c.getName(), "Food");
}

// setName 应能更新名称
TEST(CategoryTest, SetName_UpdatesName) {
    Category c("A");
    c.setName("B");
    EXPECT_EQ(c.getName(), "B");
}

// 分类名不应允许被设置为空
TEST(CategoryTest, SetName_EmptyShouldBeRejected) {
    Category c("X");
    c.setName("");
    EXPECT_EQ(c.getName(), "X");
}

// 分类名应自动去掉首尾空格
TEST(CategoryTest, SetName_ShouldTrimSpaces) {
    Category c("X");
    c.setName("  Food  ");
    EXPECT_EQ(c.getName(), "Food");
}
