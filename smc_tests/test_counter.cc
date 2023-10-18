#include <gtest/gtest.h>
#include "dummy_collection.h"
#include "../s21_counter.h"
#include "s21_matrix_oop.h"

class CounterTestFrozen :  public ::testing::Test {
protected:

    void SetUp() override{
        c_matrix.insert({std::make_pair(S21::S21Matrix(), 555), std::make_pair(S21::S21Matrix(), 666), std::make_pair(S21::S21Matrix(3,3), 1), std::make_pair(S21::S21Matrix(5,5), 4), std::make_pair(S21::S21Matrix(7,7), 2)});
        c_string_grt_stdal.insert({std::make_pair("aboba" , 35), std::make_pair("aboba", 10), std::make_pair("aboba", 1), std::make_pair("abooba", 22), std::make_pair("boba", 2), std::make_pair("boba", 11), std::make_pair("limpapopa", 1)});
        c_vector.insert({std::make_pair(std::vector<int>(15), 13), std::make_pair(std::vector<int>{1,2,3,4,5}, 55), std::make_pair(std::vector<int>(), 1)});
    }

    void TearDown() override {
        c_matrix.clear();
        c_string_grt_stdal.clear();
        c_vector.clear();

    }

    s21::Counter<S21::S21Matrix> c_matrix;
    s21::Counter<std::string, std::greater<>, std::allocator<std::pair<const std::string, int>>> c_string_grt_stdal;
    s21::Counter<std::vector<int>> c_vector;
};

TEST(CounterOnlyConstructors, key_type_inilist){

    s21::Counter<std::string> biba{"aboba", "aboba", "aboba", "abooba", "boba", "boba", "limpapopa"};
    ASSERT_EQ(biba["aboba"], 3);

    s21::Counter<S21::S21Matrix, std::greater<>, std::allocator<std::pair<const S21::S21Matrix, int>>> aboba{S21::S21Matrix(), S21::S21Matrix(), S21::S21Matrix(3,3), S21::S21Matrix(5,5), S21::S21Matrix(7,7)};
    ASSERT_EQ(aboba[S21::S21Matrix()], 3);

}

TEST(CounterOnlyConstructors, value_type_inilist){

    s21::Counter<std::string> biba{std::make_pair("aboba" , 35), std::make_pair("aboba", 10), std::make_pair("aboba", 1), std::make_pair("abooba", 22), std::make_pair("boba", 2), std::make_pair("boba", 11), std::make_pair("limpapopa", 1)};
    ASSERT_EQ(biba["aboba"], 46);

    s21::Counter<S21::S21Matrix, std::greater<>, std::allocator<std::pair<const S21::S21Matrix, int>>> aboba{std::make_pair(S21::S21Matrix(), 555), std::make_pair(S21::S21Matrix(), 666), std::make_pair(S21::S21Matrix(3,3), 1), std::make_pair(S21::S21Matrix(5,5), 4), std::make_pair(S21::S21Matrix(7,7), 2)};
    ASSERT_EQ(aboba[S21::S21Matrix()], 1225);

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

TEST_F(CounterTestFrozen, insert_pair){

    S21::S21Matrix ins(14,14);

    ASSERT_EQ((++c_matrix.begin())->second, 1225);
    ASSERT_EQ(c_matrix.insert(std::make_pair(S21::S21Matrix(), 5))->second, 1230);
    ASSERT_TRUE(!c_matrix.contains(S21::S21Matrix(20, 20)));
    ASSERT_EQ(c_matrix.insert(std::make_pair(S21::S21Matrix(20,20), 5))->second, 5);


    ASSERT_TRUE(!c_matrix.contains(ins));
    ASSERT_EQ(c_matrix.insert(std::make_pair(ins, 15))->second, 15);
    ASSERT_TRUE(c_matrix.contains(ins));
    ASSERT_EQ(c_matrix.insert(std::make_pair(ins, 444))->second, 459);


    std::string aboba("aboba");
    ASSERT_EQ((--c_string_grt_stdal.end())->first, aboba);
    ASSERT_EQ((--c_string_grt_stdal.end())->second, 46);
    ASSERT_EQ(c_string_grt_stdal.insert("aboba", 14)->second, 60);
    c_string_grt_stdal.clear();
    ASSERT_TRUE(!c_string_grt_stdal.contains(aboba));
    ASSERT_EQ(c_string_grt_stdal.insert("aboba", 2)->second, 2);
    ASSERT_ANY_THROW(c_string_grt_stdal.insert("aboba", -55));
    ASSERT_EQ(c_string_grt_stdal.insert(aboba, 2)->second, 4);

}

TEST_F(CounterTestFrozen, insert_key){

    S21::S21Matrix ins(14,14);
    ASSERT_EQ(c_matrix.insert(ins)->second, 1);
    ASSERT_EQ(c_matrix.insert(ins)->second, 2);
    ASSERT_EQ(c_matrix.insert(S21::S21Matrix(8,8))->second, 1);
    ASSERT_EQ(c_matrix.insert(S21::S21Matrix(8,8))->second, 2);

}

TEST_F(CounterTestFrozen, erase_key_count){

    ASSERT_EQ((++c_matrix.begin())->second, 1225);
    c_matrix.erase((++c_matrix.begin())->first, 1220);
    ASSERT_EQ((++c_matrix.begin())->second, 5);
    c_matrix.erase((++c_matrix.begin())->first, 55);
    ASSERT_TRUE(!c_matrix.contains(S21::S21Matrix()));

}
