#include <gtest/gtest.h>
#include <map>
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

TEST(MapOnlyConstructors, copy) {

    s21::map<int, int> aboba{std::make_pair(1, 2), std::make_pair(2, 2), std::make_pair(555, 2)};
    s21::map<int, int> abobus(aboba);

    auto it = abobus.begin();

    for (const auto &[k, v]: aboba) {
        ASSERT_EQ(it->first, k);
        ASSERT_EQ(it++->second, v);
    }
}

TEST(MapOnlyConstructors, move) {

    s21::map<int, int> aboba{std::make_pair(1, 2), std::make_pair(2, 2), std::make_pair(555, 2)};
    s21::map<int, int> abobus(aboba);
    s21::map<int, int> biba(std::move(abobus));

    auto it = biba.begin();

    for (const auto &[k, v]: aboba) {
        ASSERT_EQ(it->first, k);
        ASSERT_EQ(it++->second, v);
    }
}

TEST(MapOnlyConstructors, copy_assign) {

    s21::map<int, int> aboba{std::make_pair(1, 2), std::make_pair(2, 2), std::make_pair(555, 2)};
    s21::map<int, int> abobus;
    abobus = aboba;

    auto it = abobus.begin();

    for (const auto &[k, v]: aboba) {
        ASSERT_EQ(it->first, k);
        ASSERT_EQ(it++->second, v);
    }
}

TEST(MapOnlyConstructors, move_assign) {

    s21::map<int, int> aboba{std::make_pair(1, 2), std::make_pair(2, 2), std::make_pair(555, 2)};
    s21::map<int, int> abobus(aboba);
    s21::map<int, int> biba;
    biba = std::move(abobus);

    auto it = biba.begin();

    for (const auto &[k, v]: aboba) {
        ASSERT_EQ(it->first, k);
        ASSERT_EQ(it++->second, v);
    }
}

TEST_F(MapTestFrozen, find_key){

    ASSERT_EQ(map_m_ndd.find(S21::S21Matrix()), ++map_m_ndd.begin());

    ASSERT_EQ(map_s_s.find("aboba"), map_s_s.begin());

    ASSERT_EQ(map_d_v.find(s21::NoDefaultDummyT(1))->second, (--map_d_v.end())->second); //greater

}

TEST_F(MapTestFrozen, operatorsquares){

    ASSERT_EQ(map_s_s["aboba"], s21::set<int>());
    map_s_s["aboba"] = s21::set<int>{1,2,3,4,5};
    ASSERT_EQ(map_s_s["aboba"], s21::set<int>({1, 2, 3, 4, 5}));

    ASSERT_TRUE(!map_d_v.contains(s21::NoDefaultDummyT(55)));
    map_d_v[s21::NoDefaultDummyT(55)];
    ASSERT_EQ(map_d_v[s21::NoDefaultDummyT(55)], std::vector<int>());
    map_d_v[s21::NoDefaultDummyT(55)] = std::vector<int>{1,2,22};
    ASSERT_EQ(map_d_v[s21::NoDefaultDummyT(55)], std::vector<int>({1,2,22}));

    std::string const_key_test(map_s_s.begin()->first);
    ASSERT_EQ(map_s_s[const_key_test], s21::set<int>({1, 2, 3, 4, 5}));
    map_s_s[const_key_test] = s21::set<int>();
    ASSERT_EQ(map_s_s[const_key_test],  s21::set<int>());

}

TEST_F(MapTestFrozen, insert_key_obj){

    S21::S21Matrix m_in(9,9);

    ASSERT_TRUE(!map_m_ndd.contains(S21::S21Matrix(6,6)));
    map_m_ndd.insert(S21::S21Matrix(6,6), s21::NoDefaultDummyT(3));
    ASSERT_TRUE(map_m_ndd.contains(S21::S21Matrix(6,6)));
    ASSERT_TRUE(!map_m_ndd.contains(m_in));
    ASSERT_EQ(map_m_ndd.insert(m_in, s21::NoDefaultDummyT(44)).first->second, s21::NoDefaultDummyT(44));

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

TEST_F(MapTestFrozen, operatorat){

    ASSERT_EQ(map_s_s.at("aboba"), s21::set<int>());
    map_s_s.at("aboba") = s21::set<int>{1,2,3};
    ASSERT_EQ(map_s_s.at("aboba"), s21::set<int>({1,2,3}));
    ASSERT_ANY_THROW(map_s_s.at("asdq"));

    const auto cont_at_coverage(map_s_s);
    ASSERT_EQ(cont_at_coverage.at("aboba"), s21::set<int>({1,2,3}));

}

TEST_F(MapTestFrozen, insert_or_assigns){

    S21::S21Matrix moo;
    S21::S21Matrix moo_not_in(16,16);
    s21::NoDefaultDummyT noo(0);
    s21::set<int> ssset{5,55,555};

    ASSERT_EQ(map_s_s.at("aboba"), s21::set<int>());
    ASSERT_EQ (map_s_s.insert_or_assign("aboba", s21::set<int>({1,2,3})).first->second, s21::set<int>({1,2,3}));

    ASSERT_TRUE(!map_s_s.contains("asdq"));
    ASSERT_EQ(map_s_s.insert_or_assign("asdq", s21::set<int>({1})).first->second, s21::set<int>({1}));

    ASSERT_EQ(map_m_ndd.at(moo), s21::NoDefaultDummyT(3));
    ASSERT_EQ(map_m_ndd.insert_or_assign(moo, noo).first->second, noo);

    ASSERT_TRUE(!map_m_ndd.contains(moo_not_in));
    ASSERT_EQ(map_m_ndd.insert_or_assign(moo_not_in, noo).first->second, noo);

    ASSERT_TRUE(!map_d_v.contains(noo));
    ASSERT_EQ(map_d_v.insert_or_assign(noo, std::vector<int>(55)).first->second, std::vector<int>(55));

    ASSERT_TRUE(!map_s_s.contains("qwerty"));
    ASSERT_EQ(map_s_s.insert_or_assign("qwerty", ssset).first->second, ssset);


}

