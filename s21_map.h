#ifndef S21_CONTAINERS_S21_MAP_H_
#define S21_CONTAINERS_S21_MAP_H_

#include "s21_set.h"

namespace s21 {
    template<typename Key, typename T, typename Compare = std::less<>, typename Alloc = s21::MyTreeAllocator<std::pair<const Key, T>>>
class map : protected set<std::pair<const Key, T>, Compare, Alloc>{
public:
    using Base = set<std::pair<const Key, T>, Compare, Alloc>;
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = typename Base::iterator;
    using size_type = typename Base::size_type;

    using Base::Base;
    using Base::swap;
    using Base::begin;
    using Base::end;
    using Base::insert;
    using Base::find;
    using Base::contains;
    using Base::upper_bound;
    using Base::lower_bound;
    using Base::erase;
    using Base::size;
    using Base::empty;




};
} //namespace s21
#endif //S21_CONTAINERS_S21_MAP_H_