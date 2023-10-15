#ifndef S21_CONTAINERS_S21_MAP_H_
#define S21_CONTAINERS_S21_MAP_H_

#include "s21_set.h"

namespace s21 {
//    template<typename T>
//    class Adapter{
//        static auto adapt(T b){
//            return
//        }
//    };
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
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = typename Base::size_type;

    using iterator = typename Base::template SetIterator<false>;
    using const_iterator = typename Base::template SetIterator<true>;


    using Base::Base;
    using Base::insert;
    ~map() override = default;

    /**
    * @brief returns const reference to object mapped to key
    * if no such key exists throws
    */
    const mapped_type& at(const key_type & key) const{
        auto it = Base::find(key);
        if(it == Base::end()){
            throw std::out_of_range("No element with such key");
        }else{
            return it->second;
        }
    }

    /**
     * @brief returns reference to object mapped to key
     * if no such key exists throws
     */
    mapped_type& at(const key_type & key) {
        auto it = Base::find(key);
        if(it == Base::end()){
            throw std::out_of_range("No element with such key");
        }else{
            return it->second;
        }
    }
    /**
     * @brief Returns a reference to the value that is
     * mapped to a key equivalent to key,
     * performing an insertion if such key does not already exist.
     */
    mapped_type& operator[]( const Key& key ){
        auto it = Base::find(key);
        if(it == Base::end()){
           insert(key, T());
        }else{
            return it->second;
        }
    }
    /**
     * @brief Returns a reference to the value that is
     * mapped to a key equivalent to key,
     * performing an insertion if such key does not already exist.
     */
    mapped_type& operator[]( Key&& key ){
        auto it = Base::find(key);
        if(it == Base::end()){
            insert(std::make_pair(key, T()));
        }else{
            return it->second;
        }
    }

    /**
     * @brief attempts to insert key, mapped type pair. returns iterator to element with key and
     * true if insertion took place, false otherwise
     */
    std::pair<iterator, bool> insert(const key_type & key, const mapped_type & obj){
        return insert(std::make_pair(key,obj));
    }

    /**
     * @brief if key already exists changes object mapped to it; otherwise inserts pair\n
     * returns iterator to element in map and true if insertion took place/false if it didn't
     */
    std::pair<iterator, bool> insert_or_assign(const key_type & key, const mapped_type & obj){
        auto it = Base::find(std::make_pair(key, obj));
        if(it == Base::end()){
            return insert(std::make_pair(key, obj));
        }else{
            it->second = obj;
            return std::make_pair(it, false);
        }
    }
    /**
     * @brief if key already exists changes object mapped to it; otherwise inserts pair\n
     * returns iterator to element in map and true if insertion took place/false if it didn't
     */
    std::pair<iterator, bool> insert_or_assign(key_type && key, mapped_type && obj){
        auto it = Base::find(std::make_pair(key, obj));
        if(it == Base::end()){
            return insert(std::make_pair(key, obj));
        }else{
            it->second = obj;
            return std::make_pair(it, false);
        }
    }
    /**
     * @brief if key already exists changes object mapped to it; otherwise inserts pair\n
     * returns iterator to element in map and true if insertion took place/false if it didn't
     */
    std::pair<iterator, bool> insert_or_assign(const key_type & key, mapped_type && obj){
        auto it = Base::find(std::make_pair(key, obj));
        if(it == Base::end()){
            return insert(std::make_pair(key, obj));
        }else{
            it->second = obj;
            return std::make_pair(it, false);
        }
    }
    /**
     * @brief if key already exists changes object mapped to it; otherwise inserts pair\n
     * returns iterator to element in map and true if insertion took place/false if it didn't
     */
    std::pair<iterator, bool> insert_or_assign(key_type & key, const mapped_type & obj){
        auto it = Base::find(std::make_pair(key, obj));
        if(it == Base::end()){
            return insert(std::make_pair(key, obj));
        }else{
            it->second = obj;
            return std::make_pair(it, false);
        }
    }



protected:

    bool WrappedCompare(const value_type& lhs, const value_type& rhs) const override{
        return Base::comparator_(lhs.first, rhs.first);
    };

};
} //namespace s21
#endif //S21_CONTAINERS_S21_MAP_H_