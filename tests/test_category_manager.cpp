#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <vector>
#include "CategoryManager.h"

static int CountName(const std::vector<Category>& cats, const std::string& name) {
    return (int)std::count_if(cats.begin(), cats.end(),
                              [&](const Category& c){ return c.getName() == name; });
}

// 构造后应包含默认分类
TEST(CategoryManagerTest, HasDefaultCategories) {
    CategoryManager cm;
    EXPECT_TRUE(cm.hasCategory("Food"));
    EXPECT_TRUE(cm.hasCategory("Transport"));
    EXPECT_TRUE(cm.hasCategory("Salary"));
    EXPECT_TRUE(cm.hasCategory("Investment"));
    EXPECT_TRUE(cm.hasCategory("Entertainment"));
}

// 分类名不能为空（空串应被拒绝）
TEST(CategoryManagerTest, AddEmptyString_RejectsAndSizeUnchanged) {
    CategoryManager cm;
    auto before = cm.getCategories().size();
    cm.addCategory("");
    EXPECT_EQ(cm.getCategories().size(), before); 
    EXPECT_FALSE(cm.hasCategory(""));
}

// 添加新分类后 size 应增加
TEST(CategoryManagerTest, AddNewCategory_IncreasesSize) {
    CategoryManager cm;
    auto before = cm.getCategories().size();
    cm.addCategory("__test__");
    EXPECT_EQ(cm.getCategories().size(), before + 1);
    EXPECT_TRUE(cm.hasCategory("__test__"));
}

// 添加重复分类不应产生重复项
TEST(CategoryManagerTest, AddDuplicateCategory_DoesNotIncreaseSize) {
    CategoryManager cm;
    auto before = cm.getCategories().size();
    cm.addCategory("Food");
    EXPECT_EQ(cm.getCategories().size(), before);
    EXPECT_EQ(CountName(cm.getCategories(), "Food"), 1);
}

// 删除已存在分类后应不可再查询到
TEST(CategoryManagerTest, RemoveExistingCategory_RemovesIt) {
    CategoryManager cm;
    ASSERT_TRUE(cm.hasCategory("Food"));
    cm.removeCategory("Food");
    EXPECT_FALSE(cm.hasCategory("Food"));
}

// 默认分类不允许被删除
TEST(CategoryManagerTest, RemoveDefaultCategory_ShouldBeProtected) {
    CategoryManager cm;
    ASSERT_TRUE(cm.hasCategory("Food"));
    cm.removeCategory("Food");
    EXPECT_TRUE(cm.hasCategory("Food"));
}

// 删除不存在分类时 size 不应变化
TEST(CategoryManagerTest, RemoveNonExistingCategory_NoChange) {
    CategoryManager cm;
    auto before = cm.getCategories().size();
    cm.removeCategory("__no_such__");
    EXPECT_EQ(cm.getCategories().size(), before);
}

// 分类名不能为空
TEST(CategoryManagerTest, AddEmptyString_ShouldBeRejected) {
    CategoryManager cm;
    cm.addCategory("");
    EXPECT_FALSE(cm.hasCategory(""));
}

// 先删再加应恢复到可查询状态
TEST(CategoryManagerTest, RemoveThenAddAgain_Works) {
    CategoryManager cm;
    cm.addCategory("__temp__");
    cm.removeCategory("__temp__");
    EXPECT_FALSE(cm.hasCategory("__temp__"));
    cm.addCategory("__temp__");
    EXPECT_TRUE(cm.hasCategory("__temp__"));
}
