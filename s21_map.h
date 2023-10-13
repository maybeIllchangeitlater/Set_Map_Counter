#ifndef S21_CONTAINERS_S21_MAP_H_
#define S21_CONTAINERS_S21_MAP_H_

#include "s21_set.h"

namespace s21 {
//    template<typename Key, typename T, typename Compare, typename Alloc>
//    class map;
//
//    /**
//     * set specialization to avoid problems with usings
//     */
//    template<typename Key, typename T, typename Compare, typename Alloc>
//    class set<std::pair<const Key, T>, Compare, Alloc> {
//    public:
//        using allocator_type = Alloc;
//        using key_type = Key;
//        using value_type = std::pair<const key_type, T>;
//        using reference = value_type&;
//        using key_compare = Compare;
//        using value_compare = key_compare;
//        using const_reference = const value_type&;
//        using size_type = size_t;
//    };

    template<typename Key, typename T, typename Compare = std::less<>, typename Alloc = s21::MyTreeAllocator<std::pair<const Key, T>>>
class map : public set<std::pair<const Key, T>, Compare, Alloc>{
public:
    using Base = set<std::pair<const Key, T>, Compare, Alloc>;
    Base::key_type = Key;
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = typename Base::iterator;
    using size_type = typename Base::size_type;


    using Base::Base;
    using Base::insert;
    using Base::find;

    std::pair<iterator, bool> insert(const key_type& key, const T& obj){
        return insert(std::make_pair(key, obj));
    }
    std::pair<iterator, bool> insert_or_assign(const key_type& key, const T& obj){
        iterator it = Base::find(key);
        if(it == Base::end()){
            return insert(key, obj);
        }else{
            it->second = obj;
        }
    }




};
} //namespace s21
#endif //S21_CONTAINERS_S21_MAP_H_