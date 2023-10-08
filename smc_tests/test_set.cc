#include <gtest/gtest.h>
#include "dummy_collection.h"
#include "../s21_set.h"

TEST(default_constructor, True){
    s21::set<int> a;
    s21::set<int, std::less<>, std::allocator<int>> b;
    s21::set<s21::NoDefaultDummyT> c;
    s21::set<s21::set<int>> d;
    s21::set<s21::set<s21::set<int>>> dd;
    std::set<s21::set<std::set<s21::set<int>>>> e;
    a.insert(1);
    b.insert(1);
    c.insert(s21::NoDefaultDummyT(1));
    d.insert(a);

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
