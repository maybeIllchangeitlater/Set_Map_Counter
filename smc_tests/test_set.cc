#include <gtest/gtest.h>
#include "dummy_collection.h"
#include "../s21_set.h"
#include <string>
#include "s21_matrix_oop.h"

TEST(default_constructor, True){
    s21::set<int> a;
    s21::set<int, std::less<>, std::allocator<int>> b;
    s21::set<s21::NoDefaultDummyT> c;
    s21::set<s21::set<int>> d;
    s21::set<s21::set<s21::set<int>>> dd;
    std::set<s21::set<std::set<s21::set<int>>>> ee;
    s21::NoDefaultDummyT for_c(4);
    a.insert(1);
    b.insert(2);
    c.insert(s21::NoDefaultDummyT(3));
    c.insert(for_c);
    d.insert(a);
    dd.insert(d);

    ASSERT_EQ(*a.begin(), 1);
    ASSERT_EQ(*b.begin(), 2);
    ASSERT_EQ((*c.begin()).x, 3);
    ASSERT_EQ((*(++c.begin())).x, 4);

    ASSERT_EQ(*(d.begin()->begin()), *a.begin());
    ASSERT_EQ(*(dd.begin()->begin()->begin()), *a.begin());
}


TEST(copy_constructor, True){
    s21::NoDefaultDummyComp<S21::S21Matrix> comparator_no_def(3);
    s21::set<S21::S21Matrix, s21::NoDefaultDummyComp<S21::S21Matrix>, s21::MyTreeAllocator<S21::S21Matrix>> matrix_set(comparator_no_def);
    matrix_set.emplace(3,3);
    matrix_set.emplace(3,3);
    matrix_set.emplace(2,2);


    s21::set<S21::S21Matrix, s21::NoDefaultDummyComp<S21::S21Matrix>, s21::MyTreeAllocator<S21::S21Matrix>>matrix_set_copy(matrix_set);

    ASSERT_EQ(matrix_set.begin()->GetRows(), 2);
    ASSERT_EQ((++matrix_set.begin())->GetRows(), 3);


    ASSERT_EQ(matrix_set_copy.begin()->GetRows(), 2);
    ASSERT_EQ((++matrix_set_copy.begin())->GetRows(), 3);

}

TEST(copy_operator, True){
    s21::set<s21::NoMoveDummyT> no_move_set;
    s21::set<s21::NoMoveDummyT> no_move_set_copy;
    no_move_set.emplace(3);
    no_move_set.emplace(4);
    no_move_set.emplace(5);
    no_move_set_copy = no_move_set;

    auto it = no_move_set.begin();
    auto it2 = no_move_set_copy.begin();
    ASSERT_EQ(it->x, 3);
    ASSERT_EQ(it++->x, it2++->x);
    ASSERT_EQ(it->x, 4);
    ASSERT_EQ(it++->x, it2++->x);
    ASSERT_EQ(it->x, 5);
    ASSERT_EQ(it++->x, it2++->x);

}

TEST(move_constructor, True){
    s21::set<std::string, s21::NoCopyDummyComp<std::string>, s21::MyTreeAllocator<std::string>> string_set;
    string_set.emplace("biba");
    string_set.emplace("boba");
    string_set.emplace("aboba");
    s21::set<std::string, s21::NoCopyDummyComp<std::string>, s21::MyTreeAllocator<std::string>> string_set_copy(std::move(string_set));

    s21::set<std::string, s21::NoMoveDummyComp<std::string>, s21::MyTreeAllocator<std::string>> string_set2;
    string_set2.emplace("biba");
    string_set2.emplace("boba");
    string_set2.emplace("aboba");
    s21::set<std::string, s21::NoMoveDummyComp<std::string>, s21::MyTreeAllocator<std::string>> string_set2_copy(std::move(string_set2));

    auto it1 = string_set_copy.begin();
    auto it2 = string_set2_copy.begin();
    ASSERT_EQ(*it1++, *it2++);
    ASSERT_EQ(*it1++, *it2++);
    ASSERT_EQ(*it1++, *it2++);

}

TEST(move_operator, True){
    s21::set<std::string, s21::NoCopyDummyComp<std::string>, s21::MyTreeAllocator<std::string>> string_set;
    string_set.emplace("biba");
    string_set.emplace("boba");
    string_set.emplace("aboba");
    s21::set<std::string, s21::NoCopyDummyComp<std::string>, s21::MyTreeAllocator<std::string>> string_set_copy;

    string_set_copy = std::move(string_set);

    s21::set<std::string, s21::NoMoveDummyComp<std::string>, s21::MyTreeAllocator<std::string>> string_set2;
    string_set2.emplace("biba");
    string_set2.emplace("boba");
    string_set2.emplace("aboba");
    s21::set<std::string, s21::NoMoveDummyComp<std::string>, s21::MyTreeAllocator<std::string>> string_set2_copy(std::move(string_set2));

    auto it1 = string_set_copy.begin();
    auto it2 = string_set2_copy.begin();
    ASSERT_EQ(*it1++, *it2++);
    ASSERT_EQ(*it1++, *it2++);
    ASSERT_EQ(*it1++, *it2++);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
