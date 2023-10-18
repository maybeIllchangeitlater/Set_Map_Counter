#include <gtest/gtest.h>
#include "dummy_collection.h"
#include "../s21_counter.h"
#include "s21_matrix_oop.h"

//class CounterTestFrozen :  public ::testing::Test {
//protected:
//
//    void SetUp() override{
//        map_m_ndd.insert({std::make_pair(S21::S21Matrix(), s21::NoDefaultDummyT(3)), std::make_pair(S21::S21Matrix(15,15),s21::NoDefaultDummyT(33)), std::make_pair(S21::S21Matrix(4,4), s21::NoDefaultDummyT(333)), std::make_pair(S21::S21Matrix(7,7), s21::NoDefaultDummyT(-33))});
//        map_s_s.insert({std::make_pair("aboba", s21::set<int>()), std::make_pair("biba", s21::set<int>({1,2,3,4,5})), std::make_pair("boba", s21::set<int>({-7, 77, 887})), std::make_pair("boob", s21::set<int>({1,2}))});
//        map_d_v.insert({std::make_pair(s21::NoDefaultDummyT(3), std::vector<int>()), std::make_pair(s21::NoDefaultDummyT(1), std::vector<int>({1,2,3,4,5})), std::make_pair(s21::NoDefaultDummyT(2), std::vector<int>({1,2,3})), std::make_pair(s21::NoDefaultDummyT(53), std::vector<int>({2,2}))});
//
//    }
//
//    void TearDown() override {
//        map_m_ndd.clear();
//        map_s_s.clear();
//        map_d_v.clear();
//
//    }
//
//    s21::map<S21::S21Matrix, s21::NoDefaultDummyT> map_m_ndd;
//    s21::map<std::string, s21::set<int>, std::less<>, std::allocator<std::pair<const std::string, s21::set<int>>>> map_s_s;
//    s21::map<s21::NoDefaultDummyT, std::vector<int>, std::greater<>>map_d_v;
//};

TEST(CounterOnlyConstructors, key_type_inilist){

    s21::Counter<std::string> biba{"aboba", "aboba", "aboba", "abooba", "boba", "boba", "limpapopa"};
    ASSERT_EQ(biba["aboba"], 3);

    s21::Counter<S21::S21Matrix, std::greater<>, std::allocator<std::pair<const S21::S21Matrix, int>>> aboba{S21::S21Matrix(), S21::S21Matrix(), S21::S21Matrix(3,3), S21::S21Matrix(5,5), S21::S21Matrix(7,7)};
    ASSERT_EQ(aboba[S21::S21Matrix()], 3);

}

TEST(CounterOnlyConstructors, copy){
    s21::Counter<std::string> biba{"aboba", "aboba", "aboba", "abooba", "boba", "boba", "limpapopa"};
    s21::Counter<S21::S21Matrix, std::greater<>, std::allocator<std::pair<const S21::S21Matrix, int>>> aboba{S21::S21Matrix(), S21::S21Matrix(), S21::S21Matrix(3,3), S21::S21Matrix(5,5), S21::S21Matrix(7,7)};

    auto biba_clone(biba);
    auto aboba_clone(aboba);

    ASSERT_EQ(biba, biba_clone);
    ASSERT_EQ(aboba, aboba_clone);

}

TEST(CounterOnlyConstructors, copy_assign){

    s21::Counter<std::string> biba{"aboba", "aboba", "aboba", "abooba", "boba", "boba", "limpapopa"};
    s21::Counter<S21::S21Matrix, std::greater<>, std::allocator<std::pair<const S21::S21Matrix, int>>> aboba{S21::S21Matrix(), S21::S21Matrix(), S21::S21Matrix(3,3), S21::S21Matrix(5,5), S21::S21Matrix(7,7)};

    s21::Counter<std::string> biba_clone;
    biba_clone = biba;
    s21::Counter<S21::S21Matrix, std::greater<>, std::allocator<std::pair<const S21::S21Matrix, int>>> aboba_clone;
    aboba_clone = aboba;

    ASSERT_EQ(biba, biba_clone);
    ASSERT_EQ(aboba, aboba_clone);

}

TEST(CounterOnlyConstructors, move){

    s21::Counter<std::string> biba{"aboba", "aboba", "aboba", "abooba", "boba", "boba", "limpapopa"};
    s21::Counter<S21::S21Matrix, std::greater<>, std::allocator<std::pair<const S21::S21Matrix, int>>> aboba{S21::S21Matrix(), S21::S21Matrix(), S21::S21Matrix(3,3), S21::S21Matrix(5,5), S21::S21Matrix(7,7)};

    s21::Counter<std::string> biba_clone;
    biba_clone = biba;
    s21::Counter<S21::S21Matrix, std::greater<>, std::allocator<std::pair<const S21::S21Matrix, int>>> aboba_clone;
    aboba_clone = aboba;

    auto biba_moved(std::move(biba));
    auto aboba_moved(std::move(aboba));

    ASSERT_EQ(biba_moved, biba_clone);
    ASSERT_EQ(aboba_moved, aboba_clone);

}

TEST(CounterOnlyConstructors, move_assign){

    s21::Counter<std::string> biba{"aboba", "aboba", "aboba", "abooba", "boba", "boba", "limpapopa"};
    s21::Counter<S21::S21Matrix, std::greater<>, std::allocator<std::pair<const S21::S21Matrix, int>>> aboba{S21::S21Matrix(), S21::S21Matrix(), S21::S21Matrix(3,3), S21::S21Matrix(5,5), S21::S21Matrix(7,7)};

    s21::Counter<std::string> biba_clone;
    biba_clone = biba;
    s21::Counter<S21::S21Matrix, std::greater<>, std::allocator<std::pair<const S21::S21Matrix, int>>> aboba_clone;
    aboba_clone = aboba;

    s21::Counter<std::string>biba_moved;
    s21::Counter<S21::S21Matrix, std::greater<>, std::allocator<std::pair<const S21::S21Matrix, int>>> aboba_moved;

    biba_moved = std::move(biba);
    aboba_moved = std::move(aboba);

    ASSERT_EQ(biba_moved, biba_clone);
    ASSERT_EQ(aboba_moved, aboba_clone);

}
