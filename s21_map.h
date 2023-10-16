#ifndef S21_CONTAINERS_S21_MAP_H_
#define S21_CONTAINERS_S21_MAP_H_

#include "s21_set.h"

namespace s21 {
    namespace s21 {
        template<typename T, typename Compare, typename Alloc>
        class set;
    }

    template<typename Key, typename T, typename Compare = std::less<>, typename Alloc = MyTreeAllocator<std::pair<const Key, T>>>
class map : public set<std::pair<const Key, T>, Compare, Alloc>{
public:
    using Base = set<std::pair<const Key, T>, Compare, Alloc>;
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = typename Base::size_type;

    using iterator = typename Base::iterator;
    using const_iterator = typename Base::const_iterator;


    using Base::Base;
    using Base::insert;
    using Base::find;
    using Base::erase;

    map(const map& m) : Base(m){}

    map(map&&  m) noexcept : Base(std::move(m)){}

    map &operator=(const map& m){
        if(this == &m)
            return *this;
        map tmp(m);
        *this = std::move(tmp);
        return *this;
    }

    map &operator=(map&& m) noexcept {
        if(this == &m)
            return *this;
        Base::clear();
        if(Base::fake_root_)
            std::allocator_traits<typename Base::allocator_type_node>::deallocate
            (Base::node_alloc_, Base::fake_root_, 1);
        Base::size_ = m.size_;
        Base::fake_root_ = m.fake_root_;
        if constexpr(Base::kComparator_moves){
            Base::comparator_ = std::move(m.comparator_);
        }else{
            Base::comparator_ = m.comparator_;
        }
        Base::alloc_ = std::move(m.alloc_);
        Base::node_alloc_ = std::move(m.node_alloc_);
        m.size_ = 0;
        m.fake_root_ = nullptr;
        return *this;
    }

    ~map() override = default;

    /**
     * @brief returns iterator to element with Key key
     */
     iterator find(const key_type & key){
         ///not using std::make_pair(key, mapped_type()) here because it will not compile if mapped_type has no default constructor
        auto* crutchify = static_cast<std::pair<key_type, mapped_type>*>(::operator new(sizeof(std::pair<key_type, mapped_type>)));
        crutchify->first = key;
        auto tempIterator = find(*crutchify);
        ::operator delete(crutchify);
        return static_cast<iterator>(tempIterator);
     }
    /**
    * @brief returns const_iterator to element with Key key
    */
    const_iterator find(const key_type & key) const{
        ///not using std::make_pair(key, mapped_type()) here because it will not compile if mapped_type has no default constructor
        auto* crutchify = static_cast<std::pair<key_type, mapped_type>*>(::operator new(sizeof(std::pair<key_type, mapped_type>)));
        crutchify->first = key;
        auto it = find(*crutchify);
        ::operator delete(crutchify);
        return it;
    }
     /**
      * @brief erases element with Key key. If element doesn't exist does nothing
      */
     void erase(const key_type& key){
         ///not using std::make_pair(key, mapped_type()) here because it will not compile if mapped_type has no default constructor
         auto* crutchify = static_cast<std::pair<key_type, mapped_type>*>(::operator new(sizeof(std::pair<key_type, mapped_type>)));
         crutchify->first = key;
         erase(*crutchify);
         ::operator delete(crutchify);
     }

    /**
    * @brief returns const reference to object mapped to key
    * if no such key exists throws
    */
    const mapped_type& at(const key_type & key) const{
        const_iterator it = find(key);
        if(it == end()){
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
        auto it = find(key);
        if(it == end()){
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
        auto it = find(key);
        if(it == end()){
            return insert(std::make_pair(key, T())).first->second;
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
        auto it = find(key);
        if(it == end()){
            return insert(std::make_pair(key, T())).first->second;
        }else{
            return it->second;
        }
    }

    /**
     * @brief attempts to insert key, mapped type pair. returns iterator to element with key and
     * true if insertion took place, false otherwise
     */
    std::pair<iterator, bool> insert(const key_type & key, const mapped_type & obj){
        auto const_it =  insert(std::make_pair(key,obj));
        return std::make_pair(static_cast<iterator>(const_it.first), const_it.second);
    }

    /**
     * @brief if key already exists changes object mapped to it; otherwise inserts pair\n
     * returns iterator to element in map and true if insertion took place/false if it didn't
     */
    std::pair<iterator, bool> insert_or_assign(const key_type & key, const mapped_type & obj){
        auto it = find(std::make_pair(key, obj));
        if(it == end()){
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
        auto it = find(std::make_pair(key, obj));
        if(it == end()){
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
        auto it = find(std::make_pair(key, obj));
        if(it == end()){
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
        auto it = find(std::make_pair(key, obj));
        if(it == end()){
            return insert(std::make_pair(key, obj));
        }else{
            it->second = obj;
            return std::make_pair(it, false);
        }
    }

    bool contains(const key_type& key) const noexcept{
        return find(key) != end();
    }
//
//    iterator begin() {
//        return static_cast<iterator>(Base::begin());
//    }
//
//    iterator end() {
//        return static_cast<iterator>(Base::end());
//    }
//    const_iterator begin() const {
//        return Base::begin();
//    }
//
//    const_iterator end() const{
//        return Base::end();
//    }


protected:


    bool WrappedCompare(const value_type& lhs, const value_type& rhs) const override{
        return Base::comparator_(lhs.first, rhs.first);
    };

};
} //namespace s21
#endif //S21_CONTAINERS_S21_MAP_H_