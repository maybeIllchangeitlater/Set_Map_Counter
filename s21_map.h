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
class map : public set<std::pair<const Key, T>, std::less<>, Alloc>{
public:
    using Base = set<std::pair<const Key, T>, Compare, Alloc>;
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = typename Base::size_type;

    template< bool Const = false >
    class MapIterator : public Base::template SetIterator<Const>  {
    public:
        MapIterator() = delete;
        ~MapIterator() override = default;

    };
    using iterator = MapIterator<false>;
    using const_iterator = MapIterator<true>;


    using Base::Base;
    using Base::insert;
    ~map() override = default;


    bool SafeCompare(const value_type& lhs, const value_type& rhs) const override{
        try{
            return Base::comparator_(lhs.first, rhs.first);
        }catch(...){

//            const_cast<set<std::pair<const Key, T>, std::less<Key>, Alloc>>(Base)->clear();
            delete this;
            throw;
        }
    }

    std::pair<iterator, bool> insert(const Key& key, const T& obj){
        auto k = insert(std::make_pair(key, obj));
        return std::make_pair(iterator(k.first), true);
    }

    std::pair<iterator, bool> insert_or_assign(const Key& key, const T& obj){
        auto it = Base::find(std::make_pair(key, obj));
        if(it == Base::end()){
            auto k = insert(key, obj);
            return std::make_pair(iterator(k.first), true);

        }
//            return insert(key, obj);
//        }else{
//            iterator nit = iterator(it);
//            nit->second = obj;
//            return std::make_pair(nit, false);
//        }
    }

};
} //namespace s21
#endif //S21_CONTAINERS_S21_MAP_H_