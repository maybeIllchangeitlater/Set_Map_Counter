#include <gtest/gtest.h>
#include "dummy_collection.h"
#include "../s21_set.h"
#include <string>
#include <vector>
#include "s21_matrix_oop.h"


TEST(ConstructorsAndAssignments, DefaultConstructorMyAlloc){

    s21::set<int> a;
    s21::set<int, std::less<>> b;
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

TEST(ConstructorsAndAssignments, DefaultConstructorSTDAlloc){

    s21::set<int, s21::MyComparator<int>, std::allocator<int>> a;
    s21::set<int, std::less<>, std::allocator<int>> b;
    s21::set<s21::NoDefaultDummyT, s21::MyComparator<s21::NoDefaultDummyT>, std::allocator<s21::NoDefaultDummyT>> c;
    s21::NoDefaultDummyT for_c(4);

    a.insert(1);
    b.insert(2);
    c.insert(s21::NoDefaultDummyT(3));
    c.insert(for_c);

    ASSERT_EQ(*a.begin(), 1);
    ASSERT_EQ(*b.begin(), 2);
    ASSERT_EQ((*c.begin()).x, 3);
    ASSERT_EQ((*(++c.begin())).x, 4);
}

TEST(ConstructorsAndAssignments, ComparatorAndAllocatorBasedConstructors){

    std::allocator<S21::S21Matrix> std_matrix_alloc;
    s21::MyTreeAllocator<std::string> my_string_alloc;
    s21::NoDefaultDummyComp<S21::S21Matrix> no_default_matrix_comp(3);
    s21::NoDefaultDummyComp<std::string> no_default_string_comp(3);

    s21::set<S21::S21Matrix, s21::NoDefaultDummyComp<S21::S21Matrix>, std::allocator<S21::S21Matrix>> set_std_alloc(no_default_matrix_comp, std_matrix_alloc);
    s21::set<std::string, s21::NoDefaultDummyComp<std::string>> set_my_alloc(no_default_string_comp, my_string_alloc);
    s21::set<std::string> set_my_alloc_only(my_string_alloc);

    set_std_alloc.emplace(4,4);
    set_std_alloc.emplace(3,3);
    set_std_alloc.emplace(1,1);
    set_my_alloc.emplace("a");
    set_my_alloc.emplace("b");
    set_my_alloc.emplace("c");
    set_my_alloc_only.emplace("a");
    set_my_alloc_only.emplace("b");
    set_my_alloc_only.emplace("c");

    ASSERT_EQ(set_std_alloc.get_allocator(), std_matrix_alloc);
    ASSERT_EQ(set_my_alloc.get_allocator(), my_string_alloc);

    ASSERT_EQ(set_std_alloc.begin()->GetRows(), 1);
    ASSERT_STREQ(set_my_alloc.begin()->c_str(), "a");
    ASSERT_EQ(*set_my_alloc.begin(), *set_my_alloc_only.begin());

}

TEST(ConstructorsAndAssignments, InitlistConstructorMyAlloc) {

    std::allocator<char> char_std_alloc;
    s21::MyTreeAllocator<unsigned> unsigned_my_alloc;

    s21::set<char, s21::MyComparator<char>, std::allocator<char>>aboba {'a', 'b', 'c'};
    s21::set<unsigned> biba{1,2,3,4,5};

    s21::set<char, s21::MyComparator<char>, std::allocator<char>>aboba_alloc({'a','b','c'}, char_std_alloc);
    s21::set<unsigned> biba_alloc({1,2,3,4,5}, unsigned_my_alloc);

    ASSERT_EQ(aboba, aboba_alloc);
    ASSERT_EQ(biba, biba_alloc);

}

TEST(ConstructorsAndAssignments, CopyConstructorMyAlloc){

    s21::NoDefaultDummyComp<S21::S21Matrix> comparator_no_def(3);
    s21::set<S21::S21Matrix, s21::NoDefaultDummyComp<S21::S21Matrix>> matrix_set(comparator_no_def);
    s21::set<S21::S21Matrix, s21::NoCopyDummyComp<S21::S21Matrix>> matrix_set_no_copy_comp;

    matrix_set.emplace(3,3);
    matrix_set.emplace(3,3);
    matrix_set.emplace(2,2);
    matrix_set_no_copy_comp.emplace(3,3);
    matrix_set_no_copy_comp.emplace(3,3);
    matrix_set_no_copy_comp.emplace(2,2);

    s21::set<S21::S21Matrix, s21::NoDefaultDummyComp<S21::S21Matrix>>matrix_set_copy(matrix_set);
    s21::set<S21::S21Matrix, s21::NoCopyDummyComp<S21::S21Matrix>>matrix_set_no_copy_comp_copy(matrix_set_no_copy_comp);

    ASSERT_EQ(matrix_set.begin()->GetRows(), 2);
    ASSERT_EQ((++matrix_set.begin())->GetRows(), 3);

    ASSERT_EQ(matrix_set_copy.begin()->GetRows(), 2);
    ASSERT_EQ((++matrix_set_copy.begin())->GetRows(), 3);

    ASSERT_EQ(matrix_set_no_copy_comp.begin()->GetRows(), 2);
    ASSERT_EQ((++matrix_set_no_copy_comp.begin())->GetRows(), 3);

    ASSERT_EQ(matrix_set_no_copy_comp_copy.begin()->GetRows(), 2);
    ASSERT_EQ((++matrix_set_no_copy_comp_copy.begin())->GetRows(), 3);

}

TEST(ConstructorsAndAssignments, CopyConstructorSTDAlloc){

    s21::NoDefaultDummyComp<S21::S21Matrix> comparator_no_def(3);
    s21::set<S21::S21Matrix, s21::NoDefaultDummyComp<S21::S21Matrix>, std::allocator<S21::S21Matrix>> matrix_set(comparator_no_def);
    s21::set<S21::S21Matrix, s21::NoCopyDummyComp<S21::S21Matrix>, std::allocator<S21::S21Matrix>> matrix_set_no_copy_comp;

    matrix_set.emplace(3,3);
    matrix_set.emplace(3,3);
    matrix_set.emplace(2,2);
    matrix_set_no_copy_comp.emplace(3,3);
    matrix_set_no_copy_comp.emplace(3,3);
    matrix_set_no_copy_comp.emplace(2,2);

    s21::set<S21::S21Matrix, s21::NoDefaultDummyComp<S21::S21Matrix>, std::allocator<S21::S21Matrix>>matrix_set_copy(matrix_set);
    s21::set<S21::S21Matrix, s21::NoCopyDummyComp<S21::S21Matrix>, std::allocator<S21::S21Matrix>>matrix_set_no_copy_comp_copy(matrix_set_no_copy_comp);

    ASSERT_EQ(matrix_set.begin()->GetRows(), 2);
    ASSERT_EQ((++matrix_set.begin())->GetRows(), 3);


    ASSERT_EQ(matrix_set_copy.begin()->GetRows(), 2);
    ASSERT_EQ((++matrix_set_copy.begin())->GetRows(), 3);

    ASSERT_EQ(matrix_set_no_copy_comp.begin()->GetRows(), 2);
    ASSERT_EQ((++matrix_set_no_copy_comp.begin())->GetRows(), 3);

    ASSERT_EQ(matrix_set_no_copy_comp_copy.begin()->GetRows(), 2);
    ASSERT_EQ((++matrix_set_no_copy_comp_copy.begin())->GetRows(), 3);

}

TEST(ConstructorsAndAssignments, CopyOperatorMyAlloc){

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

TEST(ConstructorsAndAssignments, CopyOperatorSTDAlloc){
    s21::set<s21::NoMoveDummyT, s21::MyComparator<s21::NoMoveDummyT>, std::allocator<s21::NoMoveDummyT>> no_move_set;
    s21::set<s21::NoMoveDummyT, s21::MyComparator<s21::NoMoveDummyT>, std::allocator<s21::NoMoveDummyT>> no_move_set_copy;

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

TEST(ConstructorsAndAssignments, MoveConstructorMyAlloc){

    s21::set<std::string, s21::NoCopyDummyComp<std::string>> string_set;

    string_set.emplace("biba");
    string_set.emplace("boba");
    string_set.emplace("aboba");

    s21::set<std::string, s21::NoCopyDummyComp<std::string>> string_set_copy(std::move(string_set));

    s21::set<std::string, s21::NoMoveDummyComp<std::string>> string_set2;

    string_set2.emplace("biba");
    string_set2.emplace("boba");
    string_set2.emplace("aboba");

    s21::set<std::string, s21::NoMoveDummyComp<std::string>> string_set2_copy(std::move(string_set2));

    auto it1 = string_set_copy.begin();
    auto it2 = string_set2_copy.begin();

    ASSERT_EQ(*it1++, *it2++);
    ASSERT_EQ(*it1++, *it2++);
    ASSERT_EQ(*it1++, *it2++);

}


TEST(ConstructorsAndAssignments, MoveConstructorSTDAlloc){

    s21::set<std::string, s21::NoCopyDummyComp<std::string>, std::allocator<std::string>> string_set;

    string_set.emplace("biba");
    string_set.emplace("boba");
    string_set.emplace("aboba");

    s21::set<std::string, s21::NoCopyDummyComp<std::string>, std::allocator<std::string>> string_set_copy(std::move(string_set));

    s21::set<std::string, s21::NoMoveDummyComp<std::string>, std::allocator<std::string>> string_set2;

    string_set2.emplace("biba");
    string_set2.emplace("boba");
    string_set2.emplace("aboba");

    s21::set<std::string, s21::NoMoveDummyComp<std::string>, std::allocator<std::string>> string_set2_copy(std::move(string_set2));

    auto it1 = string_set_copy.begin();
    auto it2 = string_set2_copy.begin();

    ASSERT_EQ(*it1++, *it2++);
    ASSERT_EQ(*it1++, *it2++);
    ASSERT_EQ(*it1++, *it2++);

}

TEST(ConstructorsAndAssignments, MoveOperatorMyAlloc){

    s21::set<double, s21::NoCopyDummyComp<double>> set_m;

    set_m.insert(1.0);
    set_m.insert(-1.0);
    set_m.insert(0.0);

    s21::set<double, s21::NoCopyDummyComp<double>> set_m_copy;

    set_m_copy = std::move(set_m);

    s21::set<double, s21::NoMoveDummyComp<double>> set_m2;

    set_m2.insert(1.0);
    set_m2.insert(-1.0);
    set_m2.insert(0.0);

    s21::set<double, s21::NoMoveDummyComp<double>> set_m2_copy;

    set_m2_copy = std::move(set_m2);

    auto itt1 = set_m_copy.begin();
    auto itt2 = set_m2_copy.begin();

    ASSERT_EQ(*itt1++, *itt2++);
    ASSERT_EQ(*itt1++, *itt2++);
    ASSERT_EQ(*itt1++, *itt2++);
}

TEST(ConstructorsAndAssignments, MoveOperatorSTDAlloc){

    s21::set<double, s21::NoCopyDummyComp<double>, std::allocator<double>> set_m;

    set_m.insert(1.0);
    set_m.insert(-1.0);
    set_m.insert(0.0);

    s21::set<double, s21::NoCopyDummyComp<double>, std::allocator<double>> set_m_copy;

    set_m_copy = std::move(set_m);

    s21::set<double, s21::NoMoveDummyComp<double>, std::allocator<double>> set_m2;
    set_m2.insert(1.0);
    set_m2.insert(-1.0);
    set_m2.insert(0.0);
    s21::set<double, s21::NoMoveDummyComp<double>, std::allocator<double>> set_m2_copy;

    set_m2_copy = std::move(set_m2);

    auto itt1 = set_m_copy.begin();
    auto itt2 = set_m2_copy.begin();
    ASSERT_EQ(*itt1++, *itt2++);
    ASSERT_EQ(*itt1++, *itt2++);
    ASSERT_EQ(*itt1++, *itt2++);
}

TEST(ConstructorsAndAssignments, MoveAndCopySelfAssignment){
    s21::set<unsigned long long>abobus_no_copus;
    for(int i = 0; i < 10; ++i){
        abobus_no_copus.emplace(i*i);
    }
    abobus_no_copus = abobus_no_copus;
    abobus_no_copus = std::move(abobus_no_copus);

    ASSERT_EQ(*abobus_no_copus.begin(), 0);
    ASSERT_EQ(*(++abobus_no_copus.begin()), 1);
}
//
//int main(int argc, char** argv) {
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}
