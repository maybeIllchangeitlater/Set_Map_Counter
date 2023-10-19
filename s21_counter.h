#ifndef S21_CONTAINERS_S21_COUNTER_H_
#define S21_CONTAINERS_S21_COUNTER_H_

#include "s21_map.h"
#include <vector>
#include <queue>

namespace s21{
    ///attempt at Python Counter based on tree Structure. Because unlinke multiset it's cool
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
            insert(ilist);
        }

        Counter(std::initializer_list<value_type> ilist){
            insert(ilist);
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
                this->operator[](k)+=v;
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
                this->operator[](k)-=v;
                if(this->operator[](k) <= 0)
                    erase(k);
            }
            return *this;
        }
        /**
         * @brief substract count from Key. If key count drops to 0 it is erased
         * @param Key Count pair
         */
        Counter& operator-=(const value_type& value){
            erase(value.first, value.second);
            return *this;
        }
        /**
         * @brief adds count to the Key.
         * @param Key Count pair
         */
        Counter& operator+=(const value_type& value){
            insert(value.first, value.second);
            return *this;
        }

        /**
         * @brief substract count from Key. If key count drops to 0 it is erased
         * @param Key Count pair
         * @return resulting Counter
         */
        Counter operator-(const value_type& value){
            auto result(*this);
            result -= value;
            return result;
        }
        /**
         * @brief adds count to the Key.
         * @param Key Count pair
         * @return resulting Counter
         */
        Counter operator+(const value_type& value){
            auto result(*this);
            result += value;
            return result;
        }

        /**
         * @brief inserts Key count pair into the Counter. If key already exists adds count to Key\n
         * throws if count is negative
         */
        iterator insert(const value_type& value) {
            if(value.second <= 0){
                throw std::logic_error("can't insert negative amount of zero keys");
            }
            auto it = Base::find(value.first);
            if(it == Base::end()){
                return Base::insert(value).first;
            }
            it->second += value.second;
            return it;
        }
        /**
         * @brief inserts Key count pair into the Counter. If key already exists adds count to Key\n
         * throws if count is negative
         */
        iterator insert(value_type&& value) {
            if(value.second <= 0){
                throw std::logic_error("can't insert negative amount of zero keys");
            }
            auto it = Base::find(value.first);
            if(it == Base::end()){
                return Base::insert(std::move(value)).first;
            }
            it->second += value.second;
            return it;
        }
        /**
         * @brief inserts Key into the Counter.
         */
        iterator insert(const key_type& value) {
            auto it = Base::find(value);
            if(it == Base::end()){
                return Base::insert(value, 1).first;
            }
            it->second += 1;
            return it;
        }
        /**
         * @brief inserts Key into the Counter.
         */
        iterator insert(key_type&& value) {
            auto it = Base::find(value);
            if(it == Base::end()){
                return Base::insert(std::move(value), 1).first;
            }
            it->second += 1;
            return it;
        }
        /**
         * @brief inserts Key count pair into the Counter. If key already exists adds count to Key\n
         * throws if count is negative
         */
        iterator insert(const key_type& key, const int count){
            return insert(std::make_pair(key, count));

        }
        /**
         * @brief inserts Key count pair into the Counter. If key already exists adds count to Key\n
         * throws if count is negative
         */
        iterator insert(key_type&& key, const int count){
            return insert(std::make_pair(std::move(key), count));
        }
        /**
        * @brief inserts Keys into the Counter
        */
        void insert(std::initializer_list<key_type> ilist){
            for(auto& val : ilist){
                insert(std::move(val));
            }
        }
        /**
         * @brief inserts Key, count pairs to the Counter. If key already exists adds count to Key
         */
        void insert(std::initializer_list<value_type> ilist) {
            for(auto& val : ilist){
                insert(std::move(val));
            }
        }

        /**
         * @brief remove count from key element. if count drops to zero the element is removed
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
         * @brief remove count from key element. if count drops to zero the element is removed
         */
        void erase(const value_type& value){
            erase(value.first, value.second);
        }
        /**
         * @brief returns set based on Counter (no duplicates)
         */
        s21::set<Key, Compare, std::allocator<Key>> to_set() const{
            s21::set<Key, Compare, std::allocator<Key>> result;
            for(const auto &[k,v] : *this){
                result.insert(k);
            }
            return result;
        }
        /**
         * @brief returns <Key, int> map based on Counter
         */
         Base to_map() const{
             return s21::map<Key, int, Compare, Allocator>(Base::begin(), Base::end());
         }
        /**
        * @brief returns vector with duplicate elements
        */
        std::vector<Key> to_vector() const{
            size_type counter;
            std::vector<Key> result;
            for(const auto& [k,v] : *this){
                for(counter = v; counter > 0; counter--){
                    result.push_back(k);
                }
            }
            return result;
        }
        /**
         * @brief returns heap with flipped pair (count as first element)
         * @param comparator for heap.
         */
        template <typename Comparator>
        std::priority_queue<std::pair<int, Key>, std::vector<std::pair<int, Key>>, Comparator> to_heap(const Comparator& comp) const {
            std::priority_queue<std::pair<int, Key>, std::vector<std::pair<int, Key>>, Comparator> result(comp);
            for (const auto& [k, v] : *this) {
                result.push(std::make_pair(v, k));
            }
            return result;
        }
         /**
        * @brief returns heap with flipped pair (count as first element)\n
        * by default uses std::less<>
        */
         std::priority_queue<std::pair<int, Key>> to_heap() const{
             std::priority_queue<std::pair<int, Key>> result;
             for(const auto& [k,v] : *this){
                 result.push(std::make_pair(v,k));
             }
             return result;
         }

    };

}//namespace s21

#endif //S21_CONTAINERS_S21_COUNTER_H_