#ifndef S21_CONTAINERS_S21_COUNTER_H_
#define S21_CONTAINERS_S21_COUNTER_H_

#include "s21_map.h"

namespace s21{
    template<typename Key, typename Compare = MyComparator<Key>, typename Allocator = MyTreeAllocator<std::pair<const Key, int>>>
    class Counter : public map<Key, int, Compare, Allocator>{
    public:
        using Base = map<Key, int, Compare, Allocator>;
        using key_type = typename Base::key_type;
        using value_type = typename Base::value_type;
        using size_type = typename Base::size_type ;
        using key_compare = typename Base::key_compare;
        using reference = typename Base::reference;
        using const_reference = typename Base::const_reference ;
        using iterator = typename Base::iterator;
        using const_iterator = typename Base::const_iterator;

        using Base::Base;
        using Base::erase;

        Counter(std::initializer_list<key_type> ilist){
            for(auto& val : ilist){
                insert(std::move(val));
            }
        }

        Counter(const Counter &c) : Base(c) {}

        Counter(Counter &&c) noexcept: Base(std::move(c)) {}

        Counter &operator=(const Counter &c) {
            if (this == &c)
                return *this;
            Counter tmp(c);
            *this = std::move(tmp);
            return *this;
        }

        Counter &operator=(Counter &&c) noexcept {
            if (this == &c)
                return *this;
            Base::clear();
            if (Base::fake_root_)
                std::allocator_traits<typename Base::allocator_type_node>::deallocate
                        (Base::node_alloc_, Base::fake_root_, 1);
            Base::size_ = c.size_;
            Base::fake_root_ = c.fake_root_;
            if constexpr(Base::kComparator_moves) {
                Base::comparator_ = std::move(c.comparator_);
            } else {
                Base::comparator_ = c.comparator_;
            }
            Base::alloc_ = std::move(c.alloc_);
            Base::node_alloc_ = std::move(c.node_alloc_);
            c.size_ = 0;
            c.fake_root_ = nullptr;
            return *this;
        }

        ~Counter() = default;


        void swap(Counter& other){
            Base::swap(other);
        }

        void merge(Counter& other){
            Base::merge(other);
        }

        /**
         * @brief adds all elements from 2 counters and returns result as a 3rd
         */
        Counter operator+(Counter& other) const{
            Counter result(*this);
            result += other;
            return result;
        }
        /**
         * @brief adds all elements from other counter
         */
        Counter& operator+=(Counter& other){
            for(const auto&[k, v] : other){
                *this->operator[](k)+=v;
            }
            return *this;
        }
        /**
         * @brief subtracts  all elements from 2 counters and returns result as a 3rd \n
         * if element counter drops to 0 it is erased
         */
        Counter operator-(Counter& other) const{
            Counter result(*this);
            result -= other;
            return result;
        }
        /**
         * @brief subtracts all elements from other counter \n
         * if element counter drops to 0 it is erased
         */
        Counter& operator-=(Counter& other){
            for(const auto&[k, v] : other){
                *this->operator[](k)-=v;
                if(*this->operator[](k) <= 0)
                    erase(k);
            }
            return *this;
        }

        Counter& operator-=(const value_type& value){
            erase(value.first, value.second);
            return *this;
        }

        Counter& operator+=(const value_type& value){
            insert(value.first, value.second);
            return *this;
        }

        Counter operator-(const value_type& value){
            auto result(*this);
            result -= value;
            return result;
        }

        Counter operator+(const value_type& value){
            auto result(*this);
            result += value;
            return result;
        }


        iterator insert(const value_type& value) {
            if(value.second <= 0){
                throw std::logic_error("can't insert negative amount of keys");
            }
            auto it = Base::find(value.first);
            if(it == Base::end()){
                return Base::insert(value).first;
            }
            it->second += value.second;
            return it;
        }

        iterator insert(value_type&& value) {
            if(value.second <= 0){
                throw std::logic_error("can't insert negative amount of keys");
            }
            auto it = Base::find(value.first);
            if(it == Base::end()){
                return Base::insert(std::move(value)).first;
            }
            it->second += value.second;
            return it;
        }

        iterator insert(const key_type& value) {
            auto it = Base::find(value);
            if(it == Base::end()){
                return Base::insert(value, 1).first;
            }
            it->second += 1;
            return it;
        }

        iterator insert(key_type&& value) {
            auto it = Base::find(value);
            if(it == Base::end()){
                return Base::insert(std::move(value), 1).first;
            }
            it->second += 1;
            return it;
        }

        /**
         * @brief remove count from key element. if count drops to zero element is removed
         */
        void erase(const key_type& key, const int count){
            auto it = Base::find(key);
            if(it != Base::end()){
                it->second -= count;
                if(it->second <= 0)
                    erase(key);
            }
        }
        /**
         * @brief returns set based on Counter (no duplicates)
         */
        s21::set<Key, Compare, Allocator>& to_set(){
            return s21::set<Key, Compare, Allocator>(Base::begin(), Base::end());
        }
        /**
         * @brief returns <Key, int> map based on Counter
         */
         Base& to_map(){
             return s21::map<Key, int, Compare, Allocator>(Base::begin(), Base::end());
         }

        iterator insert(const key_type& key, const int count){
            return insert(std::make_pair(key, count));

        }
        iterator insert(key_type&& key, const int count){
            return insert(std::make_pair(std::move(key), count));
        }

    };

}//namespace s21

#endif //S21_CONTAINERS_S21_COUNTER_H_