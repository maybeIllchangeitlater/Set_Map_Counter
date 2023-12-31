#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "../s21_set.h"
#include "dummy_collection.h"
#include "s21_matrix_oop.h"

class SetTest :  public ::testing::Test {
protected:

     void TearDown() override {
         set_matrix_myalloc.clear();
        set_nocopy_myalloc.clear();
        set_vector_myalloc.clear();
        set_string_stdalloc.clear();
        set_nodef_stdalloc.clear();
        set_nomove_stdalloc.clear();
}

    s21::set<S21::S21Matrix> set_matrix_myalloc;
    s21::set<s21::NoCopyDummyT> set_nocopy_myalloc;
    s21::set<std::vector<int>> set_vector_myalloc;
    s21::set<std::string, std::less<>, std::allocator<std::string>> set_string_stdalloc;
    s21::set<s21::NoDefaultDummyT, s21::MyComparator<s21::NoDefaultDummyT>, std::allocator<s21::NoDefaultDummyT>> set_nodef_stdalloc;
    s21::set<s21::NoMoveDummyT, std::less<s21::NoMoveDummyT>, std::allocator<s21::NoMoveDummyT>> set_nomove_stdalloc;

};

class SetTestFrozen :  public ::testing::Test {
protected:

    void SetUp() override{
        set_matrix_myalloc.insert({S21::S21Matrix(1,1), S21::S21Matrix(2,2), S21::S21Matrix(3,3), S21::S21Matrix(15,15)});
        set_string_stdalloc.insert({"a", "aa", "ab", "ac", "ad", "b", "bb", "bc", "c", "cc"});
        set_vector_myalloc.insert({std::vector<int>{1, 2, 3}, std::vector<int>{1, 2, 3, 4, 5}, std::vector<int>{5, -5}});
        set_nodef_stdalloc.insert({s21::NoDefaultDummyT(1), s21::NoDefaultDummyT(2), s21::NoDefaultDummyT(3), s21::NoDefaultDummyT(33)});
        set_nomove_stdalloc.insert(
                {s21::NoMoveDummyT(1), s21::NoMoveDummyT(2), s21::NoMoveDummyT(3), s21::NoMoveDummyT(33)});
        set_nocopy_myalloc.emplace(3);
        set_nocopy_myalloc.emplace(2);
        set_nocopy_myalloc.emplace(33);
        set_nocopy_myalloc.emplace(1);

    }

    void TearDown() override {
        set_matrix_myalloc.clear();
        set_nocopy_myalloc.clear();
        set_vector_myalloc.clear();
        set_string_stdalloc.clear();
        set_nodef_stdalloc.clear();
        set_nomove_stdalloc.clear();
    }

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

}

TEST_F(SetTest, insert_rvalue){

    set_matrix_myalloc.find(S21::S21Matrix(3,3));
    set_matrix_myalloc.emplace(3,3);
    set_matrix_myalloc.find(S21::S21Matrix(3,3));
    set_matrix_myalloc.insert(S21::S21Matrix(3,3));
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

TEST_F(SetTest, insert_iterator){

    std::vector<std::string>ins_s{"aboba", "biba", "boba", "baboba"};

    std::vector<S21::S21Matrix>ins_m;
    ins_m.push_back(S21::S21Matrix(3,3));
    ins_m.push_back(S21::S21Matrix(4,4));
    ins_m.push_back(S21::S21Matrix(2,2));
    ins_m.push_back(S21::S21Matrix(2,2));
    ins_m.push_back(S21::S21Matrix(4,4));

    std::vector<std::vector<int>>ins_v;
    ins_v.push_back(std::vector<int>{1,2,3,3,3,1,2});
    ins_v.push_back(std::vector<int>{1,2,3,3,3,1,2});
    ins_v.push_back(std::vector<int>{1,2,3,3,4,5});
    ins_v.push_back(std::vector<int>{1});

    std::vector<s21::NoDefaultDummyT>ins_ndd;
    ins_ndd.push_back(s21::NoDefaultDummyT(3));
    ins_ndd.push_back(s21::NoDefaultDummyT(3));
    ins_ndd.push_back(s21::NoDefaultDummyT(3));
    ins_ndd.push_back(s21::NoDefaultDummyT(4));
    ins_ndd.push_back(s21::NoDefaultDummyT(5));


    s21::set<s21::NoMoveDummyT>ins_nmd;
    s21::NoMoveDummyT a(3);
    s21::NoMoveDummyT b (4);
    s21::NoMoveDummyT c (5);
    ins_nmd.insert(a);
    ins_nmd.insert(b);
    ins_nmd.insert(c);

    //tests start here

    set_string_stdalloc.insert(ins_s.begin(), ins_s.end());

    set_matrix_myalloc.insert(ins_m.begin(), ins_m.end());

    set_vector_myalloc.insert(ins_v.begin(), ins_v.end());

    set_nodef_stdalloc.insert(ins_ndd.begin(), ins_ndd.end());

    set_nomove_stdalloc.insert(ins_nmd.begin(), ins_nmd.end());

    ASSERT_EQ(set_string_stdalloc.size(), 4);
    ASSERT_STREQ(set_string_stdalloc.begin()->c_str(), "aboba");

    ASSERT_EQ(set_matrix_myalloc.size(), 3);
    ASSERT_EQ(set_matrix_myalloc.begin()->GetRows(), 2);

    ASSERT_EQ(*set_vector_myalloc.begin(), ins_v.back());
    ASSERT_EQ(set_vector_myalloc.size(), 3);

    ASSERT_EQ(set_nodef_stdalloc.begin()->x, 3);
    ASSERT_EQ(set_nodef_stdalloc.size(), 3);

    auto it = set_nomove_stdalloc.begin();
    for(const auto& v : ins_nmd){
        ASSERT_EQ(v.x, it++->x);
    }

}

TEST_F(SetTest, insert_initlist){

    set_string_stdalloc.insert({"aboba", "biba", "boba", "booba", "goba", "biba"});

    set_matrix_myalloc.insert({S21::S21Matrix(3,3), S21::S21Matrix(2,2), S21::S21Matrix(3,3), S21::S21Matrix(4,4)});

    set_vector_myalloc.insert({std::vector<int>{1,2,3,4}, std::vector<int>{1,2,3,4,5}, std::vector<int>{1,2,3,4,5,6}});

    set_nodef_stdalloc.insert({s21::NoDefaultDummyT(3), s21::NoDefaultDummyT(4), s21::NoDefaultDummyT(5)});

    s21::NoMoveDummyT a(3);
    s21::NoMoveDummyT b (4);
    s21::NoMoveDummyT c (5);
    set_nomove_stdalloc.insert({a,b,c});

    ASSERT_STREQ(set_string_stdalloc.begin()->c_str(), "aboba");
    ASSERT_EQ(set_string_stdalloc.size(), 5);

    int i = 2;
    for(const auto& v: set_matrix_myalloc){
        ASSERT_EQ(v.GetRows(), i++);
    }

    ASSERT_EQ(set_vector_myalloc.begin()->back(), 4);
    ASSERT_EQ((++set_vector_myalloc.begin())->back(), 5);
    ASSERT_EQ(set_vector_myalloc.size(), 3);


    ASSERT_EQ(set_nodef_stdalloc.begin()->x, 3);
    ASSERT_EQ((++set_nodef_stdalloc.begin())->x, 4);
    ASSERT_EQ(set_nodef_stdalloc.size(), 3);

    ASSERT_EQ(set_nomove_stdalloc.begin()->x, 3);
    ASSERT_EQ((++set_nomove_stdalloc.begin())->x, 4);
    ASSERT_EQ(set_nomove_stdalloc.size(), 3);

}

TEST_F(SetTest, emplace){

    set_matrix_myalloc.emplace(1,1);
    set_matrix_myalloc.emplace();
    set_matrix_myalloc.emplace(*set_matrix_myalloc.begin()); //KEKW

    set_string_stdalloc.emplace();
    set_string_stdalloc.emplace("aboba");
    set_string_stdalloc.emplace(*++set_string_stdalloc.begin());
    set_string_stdalloc.emplace(10, 'a');

    s21::set<int>for_vec{1,2,3,4,5};
    set_vector_myalloc.emplace(10);
    set_vector_myalloc.emplace(std::vector<int>{1,2,3,4});
    set_vector_myalloc.emplace(for_vec.begin(), for_vec.end());
    set_vector_myalloc.emplace(*set_vector_myalloc.begin());

    set_nodef_stdalloc.emplace(3);
    set_nodef_stdalloc.emplace(*set_nodef_stdalloc.begin());

    set_nomove_stdalloc.emplace();
    set_nomove_stdalloc.emplace(3);
    set_nomove_stdalloc.emplace((++set_vector_myalloc.begin())->back());

    set_nocopy_myalloc.emplace();
    set_nocopy_myalloc.emplace(3);
    set_nocopy_myalloc.emplace(s21::NoCopyDummyT(4));

    ASSERT_EQ(set_matrix_myalloc.begin()->GetRows(), 1);
    ASSERT_EQ(set_matrix_myalloc.size(), 2);
    ASSERT_TRUE(set_matrix_myalloc.begin()->GetRows() < (++set_matrix_myalloc.begin())->GetRows());

    ASSERT_TRUE(set_string_stdalloc.begin()->empty());
    ASSERT_EQ((++set_string_stdalloc.begin())->length(), 10);
    ASSERT_EQ((++set_string_stdalloc.begin())->back(), 'a');

    auto it = set_vector_myalloc.begin();
    ASSERT_EQ(it->size(), 10);
    ASSERT_EQ(it->back(), 0);
    ++it;
    ++it;
    int i = 0;
    for(const auto& v: for_vec){
        ASSERT_EQ(v, (*it)[i++]);
    }

    ASSERT_EQ(set_nocopy_myalloc.size(), 3);
    ASSERT_EQ(set_nomove_stdalloc.size(), set_nocopy_myalloc.size());

}

TEST_F(SetTestFrozen, erase_contains){

    ASSERT_TRUE(set_string_stdalloc.contains("ac"));
    set_string_stdalloc.erase("ac");
    ASSERT_TRUE(!set_string_stdalloc.contains("ac"));
    set_string_stdalloc.erase(--set_string_stdalloc.end());
    ASSERT_TRUE(!set_string_stdalloc.contains("cc"));

    ASSERT_TRUE(set_matrix_myalloc.contains(S21::S21Matrix(1,1)));
    set_matrix_myalloc.erase(S21::S21Matrix(1,1));
    ASSERT_TRUE(!set_matrix_myalloc.contains(S21::S21Matrix(1,1)));
    set_matrix_myalloc.erase(--set_matrix_myalloc.end());

    ASSERT_TRUE(set_vector_myalloc.contains(std::vector<int>{1,2,3,4,5}));
    set_vector_myalloc.erase(std::vector<int>{1,2,3,4,5});
    ASSERT_TRUE(!set_vector_myalloc.contains(std::vector<int>{1,2,3,4,5}));
    set_vector_myalloc.erase(set_vector_myalloc.begin());
    ASSERT_TRUE(!set_vector_myalloc.contains(std::vector<int>{1,2,3}));

    ASSERT_TRUE(set_nodef_stdalloc.contains(s21::NoDefaultDummyT(1)));
    set_nodef_stdalloc.erase(s21::NoDefaultDummyT(1));
    ASSERT_TRUE(!set_nodef_stdalloc.contains(s21::NoDefaultDummyT(1)));
    set_nodef_stdalloc.erase(--set_nodef_stdalloc.end());
    ASSERT_TRUE(!set_nodef_stdalloc.contains(s21::NoDefaultDummyT(33)));

    ASSERT_TRUE(set_nocopy_myalloc.contains(s21::NoCopyDummyT(1)));
    set_nocopy_myalloc.erase(s21::NoCopyDummyT(1));
    ASSERT_TRUE(!set_nocopy_myalloc.contains(s21::NoCopyDummyT(1)));
    set_nocopy_myalloc.erase(--set_nocopy_myalloc.end());
    ASSERT_TRUE(!set_nocopy_myalloc.contains(s21::NoCopyDummyT(33)));

    ASSERT_TRUE(set_nomove_stdalloc.contains(s21::NoMoveDummyT(1)));
    set_nomove_stdalloc.erase(s21::NoMoveDummyT(1));
    ASSERT_TRUE(!set_nomove_stdalloc.contains(s21::NoMoveDummyT(1)));
    set_nomove_stdalloc.erase(--set_nomove_stdalloc.end());
    ASSERT_TRUE(!set_nomove_stdalloc.contains(s21::NoMoveDummyT(33)));

}

TEST_F(SetTestFrozen, erase_range){
    set_matrix_myalloc.erase(set_matrix_myalloc.begin(), set_matrix_myalloc.end());
    ASSERT_TRUE(set_matrix_myalloc.empty());

    set_string_stdalloc.erase(set_string_stdalloc.begin(), --set_string_stdalloc.end());
    ASSERT_STREQ((--set_string_stdalloc.end())->c_str(), "cc");

}

TEST_F(SetTest, merge){
    set_string_stdalloc.insert({"a", "aa", "ab", "ac", "ad", "b", "bb", "bc", "c", "cc"});
    s21::set<std::string, std::less<>, std::allocator<std::string>> b{"a", "aa", "aaa", "b", "bb", "bbb", "c", "cc", "ccc"};

    std::set<std::string> s_from{"a", "aa", "ab", "ac", "ad", "b", "bb", "bc", "c", "cc"};
    std::set<std::string>s_to{"a", "aa", "aaa", "b", "bb", "bbb", "c", "cc", "ccc"};

    s21::set<int> i_from{1,3,5,7,9,13,15};
    s21::set<int> i_to{1,2,3,4,5,6,7};

    std::set<int> i_fromstd{1,3,5,7,9,13,15};
    std::set<int> i_tostd{1,2,3,4,5,6,7};


    b.merge(set_string_stdalloc);
    s_to.merge(s_from);

    i_to.merge(i_from);
    i_tostd.merge(i_fromstd);

    auto it1 = b.begin();
    auto it2 = set_string_stdalloc.begin();
    auto it3 = i_from.begin();
    auto it4 = i_to.begin();
    for(const auto& v: s_to){
        ASSERT_EQ(*it1++, v);
    }
    for(const auto& v: s_from){
        ASSERT_EQ(*it2++, v);
    }
    for(const auto& v: i_fromstd){
        ASSERT_EQ(*it3++, v);
    }
    for(const auto& v: i_tostd){
        ASSERT_EQ(*it4++, v);
    }


}

TEST_F(SetTestFrozen, clear){
    set_matrix_myalloc.clear();
    ASSERT_TRUE(set_matrix_myalloc.empty());
    set_matrix_myalloc.emplace(S21::S21Matrix());
    ASSERT_EQ(set_matrix_myalloc.begin()->GetRows(), 5);

    set_string_stdalloc.clear();
    ASSERT_TRUE(set_string_stdalloc.empty());
    set_string_stdalloc.emplace("uWu");
    ASSERT_STREQ(set_string_stdalloc.begin()->c_str(), "uWu");

    set_vector_myalloc.clear();
    ASSERT_TRUE(set_vector_myalloc.empty());
    set_vector_myalloc.insert(std::vector<int>{1,1,1,1});
    ASSERT_EQ(std::count(set_vector_myalloc.begin()->begin(), set_vector_myalloc.begin()->end(), 1), 4);

    set_nodef_stdalloc.clear();
    ASSERT_TRUE(set_nodef_stdalloc.empty());
    set_nodef_stdalloc.emplace(123);
    ASSERT_EQ(set_nodef_stdalloc.begin()->x, 123);

    set_nomove_stdalloc.clear();
    ASSERT_TRUE(set_nomove_stdalloc.empty());
    set_nomove_stdalloc.emplace(123);
    ASSERT_EQ(set_nomove_stdalloc.begin()->x, 123);

    set_nocopy_myalloc.clear();
    ASSERT_TRUE(set_nocopy_myalloc.empty());
    set_nocopy_myalloc.emplace(123);
    ASSERT_EQ(set_nocopy_myalloc.begin()->x, 123);

}


TEST_F(SetTestFrozen, upper_bound){

    ASSERT_EQ(*set_matrix_myalloc.upper_bound(S21::S21Matrix(4,4)), *std::upper_bound(set_matrix_myalloc.begin(), set_matrix_myalloc.end(), S21::S21Matrix(4,4)));
    ASSERT_EQ(*set_string_stdalloc.upper_bound("aaa"), *std::upper_bound(set_string_stdalloc.begin(), set_string_stdalloc.end(), "aaa"));
    ASSERT_EQ(*set_vector_myalloc.upper_bound(std::vector<int>{4}), *std::upper_bound(set_vector_myalloc.begin(), set_vector_myalloc.end(), std::vector<int>{4}));
    ASSERT_EQ(set_nodef_stdalloc.upper_bound(s21::NoDefaultDummyT(15))->x, std::upper_bound(set_nomove_stdalloc.begin(), set_nomove_stdalloc.end(), s21::NoMoveDummyT(15))->x);
    ASSERT_EQ(set_nocopy_myalloc.upper_bound(s21::NoCopyDummyT(15))->x, 33);

}

TEST_F(SetTestFrozen, lower_bound){

    ASSERT_EQ(*set_matrix_myalloc.lower_bound(S21::S21Matrix(4,4)), *std::lower_bound(set_matrix_myalloc.begin(), set_matrix_myalloc.end(), S21::S21Matrix(4,4)));
    ASSERT_EQ(*set_string_stdalloc.lower_bound("aaa"), *std::lower_bound(set_string_stdalloc.begin(), set_string_stdalloc.end(), "aaa"));
    ASSERT_EQ(*set_vector_myalloc.lower_bound(std::vector<int>{4}), *std::lower_bound(set_vector_myalloc.begin(), set_vector_myalloc.end(), std::vector<int>{4}));
    ASSERT_EQ(set_nodef_stdalloc.lower_bound(s21::NoDefaultDummyT(15))->x, std::lower_bound(set_nomove_stdalloc.begin(), set_nomove_stdalloc.end(), s21::NoMoveDummyT(15))->x);
    ASSERT_EQ(set_nocopy_myalloc.lower_bound(s21::NoCopyDummyT(15))->x, 33);

}

TEST_F(SetTestFrozen, find){

    ASSERT_EQ(set_matrix_myalloc.find(S21::S21Matrix(1,1)), set_matrix_myalloc.begin());
    ASSERT_EQ(--set_string_stdalloc.end(), set_string_stdalloc.find("cc"));
    ASSERT_EQ(set_vector_myalloc.end(), set_vector_myalloc.find(std::vector<int>{123,123,123}));
    ASSERT_EQ(set_nocopy_myalloc.find(s21::NoCopyDummyT(1)), set_nocopy_myalloc.begin());
    ASSERT_EQ(set_nomove_stdalloc.find(s21::NoMoveDummyT(1)), set_nomove_stdalloc.begin());
    ASSERT_EQ(set_nodef_stdalloc.find(s21::NoDefaultDummyT(1)), set_nodef_stdalloc.begin());

}

TEST_F(SetTestFrozen, swap){
    auto m_cpy(set_matrix_myalloc);
    s21::set<S21::S21Matrix> matrix2{S21::S21Matrix()};
    auto m2_cpy(matrix2);

    set_matrix_myalloc.swap(matrix2);

    ASSERT_EQ(m_cpy, matrix2);
    ASSERT_EQ(m2_cpy, set_matrix_myalloc);
}



//set_matrix_myalloc.insert({S21::S21Matrix(1,1), S21::S21Matrix(2,2), S21::S21Matrix(3,3), S21::S21Matrix(15,15)});
//set_string_stdalloc.insert({"a", "aa", "ab", "ac", "ad", "b", "bb", "bc", "c", "cc"});
//set_vector_myalloc.insert({std::vector<int>{1, 2, 3}, std::vector<int>{1, 2, 3, 4, 5}, std::vector<int>{5, -5}});
//set_nodef_stdalloc.insert({s21::NoDefaultDummyT(1), s21::NoDefaultDummyT(2), s21::NoDefaultDummyT(3), s21::NoDefaultDummyT(33)});
//set_nomove_stdalloc.insert(
//{s21::NoMoveDummyT(1), s21::NoMoveDummyT(2), s21::NoMoveDummyT(3), s21::NoMoveDummyT(33)});
//set_nocopy_myalloc.emplace(3);
//set_nocopy_myalloc.emplace(2);
//set_nocopy_myalloc.emplace(33);
//set_nocopy_myalloc.emplace(1);

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}