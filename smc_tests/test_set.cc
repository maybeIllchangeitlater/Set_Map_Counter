#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "../s21_set.h"
#include "dummy_collection.h"
#include "s21_matrix_oop.h"

//struct SetTest {
//    s21::set<S21::S21Matrix, std::greater<>, std::allocator<S21::S21Matrix>> set_matrix_stdalloc;
//    s21::set<const char*> set_cstring_myalloc;
//    s21::set<s21::NoDefaultDummyT, s21::MyComparator<s21::NoDefaultDummyT>, std::allocator<s21::NoDefaultDummyT>> set_nodef_stdalloc;
//    s21::set<s21::NoCopyDummyT> set_nocopy_myalloc;
//    s21::set<s21::NoMoveDummyT, std::less<s21::NoMoveDummyT>, std::allocator<s21::NoMoveDummyT>> set_nomove_stdalloc;
//    s21::set<std::vector<int>> set_vector_myalloc;
//};
//

class SetTest :  public ::testing::Test {
protected:
//    void SetUp() override {
//        set_matrix_myalloc.clear();
//        set_nocopy_myalloc.clear();
//        set_vector_myalloc.clear();
//        set_string_stdalloc.clear();
//        set_nodef_stdalloc.clear();
//        set_nomove_stdalloc.clear();
//    }

//     void TearDown() override {
//         set_matrix_myalloc.clear();
//        set_nocopy_myalloc.clear();
//        set_vector_myalloc.clear();
//        set_string_stdalloc.clear();
//        set_nodef_stdalloc.clear();
//        set_nomove_stdalloc.clear();
//}

    s21::set<S21::S21Matrix> set_matrix_myalloc;
    s21::set<s21::NoCopyDummyT> set_nocopy_myalloc;
    s21::set<std::vector<int>> set_vector_myalloc;
    s21::set<std::string, std::less<>, std::allocator<std::string>> set_string_stdalloc;
    s21::set<s21::NoDefaultDummyT, s21::MyComparator<s21::NoDefaultDummyT>, std::allocator<s21::NoDefaultDummyT>> set_nodef_stdalloc;
    s21::set<s21::NoMoveDummyT, std::less<s21::NoMoveDummyT>, std::allocator<s21::NoMoveDummyT>> set_nomove_stdalloc;

};

TEST_F(SetTest, insert_lvalue){
    S21::S21Matrix ins_m1(3,3);
    S21::S21Matrix ins_m2(4,4);
    S21::S21Matrix ins_m3(3,3);
    set_matrix_myalloc.insert(ins_m1);
    set_matrix_myalloc.insert(ins_m2);
    set_matrix_myalloc.insert(ins_m3);

    std::vector<int> ins_v1{1,2,3,4};
    std::vector<int> ins_v2{1,2,3,4};
    std::vector<int> ins_v3{1,2,3,4,4};
    set_vector_myalloc.insert(ins_v1);
    set_vector_myalloc.insert(ins_v2);
    set_vector_myalloc.insert(ins_v3);

    std::string ins_cs1 = "aboba";
    std::string ins_cs2 = "aboba";
    std::string ins_cs3 = "boba";
    set_string_stdalloc.insert(ins_cs1);
    set_string_stdalloc.insert(ins_cs2);
    set_string_stdalloc.insert(ins_cs3);

    s21::NoDefaultDummyT ins_ndd1(3);
    s21::NoDefaultDummyT ins_ndd2(3);
    s21::NoDefaultDummyT ins_ndd3(4);
    set_nodef_stdalloc.insert(ins_ndd1);
    set_nodef_stdalloc.insert(ins_ndd2);
    set_nodef_stdalloc.insert(ins_ndd3);

    s21::NoMoveDummyT ins_nmd1(3);
    s21::NoMoveDummyT ins_nmd2(4);
    s21::NoMoveDummyT ins_nmd3(3);
    set_nomove_stdalloc.insert(ins_nmd1);
    set_nomove_stdalloc.insert(ins_nmd2);
    set_nomove_stdalloc.insert(ins_nmd3);

    ASSERT_EQ(set_matrix_myalloc.size(), set_vector_myalloc.size());
    ASSERT_EQ(set_vector_myalloc.size(), set_string_stdalloc.size());
    ASSERT_EQ(set_string_stdalloc.size(), set_nodef_stdalloc.size());
    ASSERT_EQ(set_nodef_stdalloc.size(), set_nomove_stdalloc.size());
    ASSERT_EQ(set_nomove_stdalloc.size(), 2);


    set_matrix_myalloc.emplace(3,3);
    set_matrix_myalloc.insert(ins_m1);
    set_matrix_myalloc.insert(S21::S21Matrix(3,3));
    set_matrix_myalloc.insert(S21::S21Matrix(4,4));

    set_vector_myalloc.insert(std::vector<int>{1,2,3,4});
    set_vector_myalloc.insert(std::vector<int>{1,2,3,4});
    set_vector_myalloc.insert(std::vector<int>{1,2,3,4,5});

    set_string_stdalloc.insert("aboba");
    set_string_stdalloc.insert("aboba");
    set_string_stdalloc.insert("boba");

    set_nocopy_myalloc.insert(s21::NoCopyDummyT(3));
    set_nocopy_myalloc.insert(s21::NoCopyDummyT(3));
    set_nocopy_myalloc.insert(s21::NoCopyDummyT(4));

    set_nodef_stdalloc.insert(s21::NoDefaultDummyT(3));
    set_nodef_stdalloc.insert(s21::NoDefaultDummyT(3));
    set_nodef_stdalloc.insert(s21::NoDefaultDummyT(4));

    auto smm_it = set_matrix_myalloc.begin();
    auto svm_it = set_vector_myalloc.begin();
    auto sss_it = set_string_stdalloc.begin();
    auto snm_it = set_nocopy_myalloc.begin();
    auto sns_it = set_nodef_stdalloc.begin();

    ASSERT_EQ(smm_it++->GetRows(), 3);
    ASSERT_EQ(smm_it->GetRows(), 4);

    ASSERT_EQ((*svm_it)[3], 4);
    ASSERT_EQ((*++svm_it)[4], 4);
    ASSERT_EQ((*++svm_it)[4], 5);

    ASSERT_STREQ(sss_it->c_str(), "aboba");
    ASSERT_STREQ((++sss_it)->c_str(), "boba");

    ASSERT_EQ(snm_it->x, 3);
    ASSERT_EQ((++snm_it)->x, 4);

    ASSERT_EQ(sns_it->x, 3);
    ASSERT_EQ((++sns_it)->x, 4);


}

TEST_F(SetTest, insert_rvalue){
    set_matrix_myalloc.find(S21::S21Matrix(3,3));
    set_matrix_myalloc.emplace(3,3);
    set_matrix_myalloc.find(S21::S21Matrix(3,3));
    S21::S21Matrix ins_m1(3,3);
    set_matrix_myalloc.insert(ins_m1);
    set_matrix_myalloc.insert(S21::S21Matrix(3,3));
    set_matrix_myalloc.insert(S21::S21Matrix(4,4));

    set_vector_myalloc.insert(std::vector<int>{1,2,3,4});
    set_vector_myalloc.insert(std::vector<int>{1,2,3,4});
    set_vector_myalloc.insert(std::vector<int>{1,2,3,4,5});

    set_string_stdalloc.insert("aboba");
    set_string_stdalloc.insert("aboba");
    set_string_stdalloc.insert("boba");

    set_nocopy_myalloc.insert(s21::NoCopyDummyT(3));
    set_nocopy_myalloc.insert(s21::NoCopyDummyT(3));
    set_nocopy_myalloc.insert(s21::NoCopyDummyT(4));

    set_nodef_stdalloc.insert(s21::NoDefaultDummyT(3));
    set_nodef_stdalloc.insert(s21::NoDefaultDummyT(3));
    set_nodef_stdalloc.insert(s21::NoDefaultDummyT(4));

    auto smm_it = set_matrix_myalloc.begin();
    auto svm_it = set_vector_myalloc.begin();
    auto sss_it = set_string_stdalloc.begin();
    auto snm_it = set_nocopy_myalloc.begin();
    auto sns_it = set_nodef_stdalloc.begin();

    ASSERT_EQ(smm_it++->GetRows(), 3);
    ASSERT_EQ(smm_it->GetRows(), 4);

    ASSERT_EQ((*svm_it)[3], 4);
    ASSERT_EQ((*++svm_it)[4], 5);

    ASSERT_STREQ(sss_it->c_str(), "aboba");
    ASSERT_STREQ((++sss_it)->c_str(), "boba");

    ASSERT_EQ(snm_it->x, 3);
    ASSERT_EQ((++snm_it)->x, 4);

    ASSERT_EQ(sns_it->x, 3);
    ASSERT_EQ((++sns_it)->x, 4);

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}