#include <gtest/gtest.h>
#include "dummy_collection.h"
#include "../s21_map.h"
#include <string>
#include <vector>
#include "s21_matrix_oop.h"


class MapTestFrozen :  public ::testing::Test {
protected:

    void SetUp() override{
        map_m_ndd.insert({std::make_pair(S21::S21Matrix(), s21::NoDefaultDummyT(3)), std::make_pair(S21::S21Matrix(15,15),s21::NoDefaultDummyT(33)), std::make_pair(S21::S21Matrix(4,4), s21::NoDefaultDummyT(333)), std::make_pair(S21::S21Matrix(7,7), s21::NoDefaultDummyT(-33))});
        map_s_s.insert({std::make_pair("aboba", s21::set<int>()), std::make_pair("biba", s21::set<int>({1,2,3,4,5})), std::make_pair("boba", s21::set<int>({-7, 77, 887})), std::make_pair("boob", s21::set<int>({1,2}))});
        map_d_v.insert({std::make_pair(s21::NoDefaultDummyT(3), std::vector<int>()), std::make_pair(s21::NoDefaultDummyT(1), std::vector<int>({1,2,3,4,5})), std::make_pair(s21::NoDefaultDummyT(2), std::vector<int>({1,2,3})), std::make_pair(s21::NoDefaultDummyT(53), std::vector<int>({2,2}))});

    }

    void TearDown() override {
        map_m_ndd.clear();
        map_s_s.clear();
        map_d_v.clear();
    }

    s21::map<S21::S21Matrix, s21::NoDefaultDummyT> map_m_ndd;
    s21::map<std::string, s21::set<int>, std::less<>, std::allocator<std::pair<const std::string, s21::set<int>>>> map_s_s;
    s21::map<s21::NoDefaultDummyT, std::vector<int>, std::greater<>>map_d_v;

};

TEST_F(MapTestFrozen, find_key){

    ASSERT_EQ(map_m_ndd.find(S21::S21Matrix()), ++map_m_ndd.begin());
    ASSERT_EQ(map_s_s.find("aboba"), map_s_s.begin());
    ASSERT_EQ(map_d_v.find(s21::NoDefaultDummyT(53)), --map_d_v.end());

}

TEST_F(MapTestFrozen, operatorsquares){

    ASSERT_EQ(map_s_s["aboba"], s21::set<int>());

}

TEST_F(MapTestFrozen, insert_key_obj){

    ASSERT_TRUE(!map_m_ndd.contains(S21::S21Matrix(6,6)));
    map_m_ndd.insert(S21::S21Matrix(6,6), s21::NoDefaultDummyT(3));
    ASSERT_TRUE(map_m_ndd.contains(S21::S21Matrix(6,6)));

    ASSERT_TRUE(!map_s_s.contains("abobus"));
    map_s_s.insert("abobus", s21::set<int>({-555, -655}));
    ASSERT_TRUE(map_s_s.contains("abobus"));

    ASSERT_TRUE(!map_d_v.contains(s21::NoDefaultDummyT(42)));
    map_d_v.insert(s21::NoDefaultDummyT(42), std::vector<int>{5,5,3,3});
    ASSERT_TRUE(map_d_v.contains(s21::NoDefaultDummyT(42)));

}

TEST_F(MapTestFrozen, erase_key){

    ASSERT_TRUE(map_m_ndd.contains(S21::S21Matrix()));
    map_m_ndd.erase(S21::S21Matrix());
    ASSERT_TRUE(!map_m_ndd.contains(S21::S21Matrix()));

    ASSERT_TRUE(map_s_s.contains("aboba"));
    map_s_s.erase("aboba");
    ASSERT_TRUE(!map_s_s.contains("aboba"));

    ASSERT_TRUE(map_d_v.contains(s21::NoDefaultDummyT(53)));
    map_d_v.erase(s21::NoDefaultDummyT(53));
    ASSERT_TRUE(!map_d_v.contains(s21::NoDefaultDummyT(53)));

}

