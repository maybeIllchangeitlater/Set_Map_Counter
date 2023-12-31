#ifndef S21_CONTAINERS_S21_MAP_H_
#define S21_CONTAINERS_S21_MAP_H_

#include "s21_set.h"

namespace s21 {
    template<typename Key, typename T, typename Compare = MyComparator<const Key>, typename Alloc = MyTreeAllocator<std::pair<const Key, T>>>
    class map : public set<std::pair<const Key, T>, Compare, Alloc> {
    public:
        using Base = set<std::pair<const Key, T>, Compare, Alloc>;
        using key_type = Key;
        using mapped_type = T;
        using key_compare = Compare;
        using value_type = std::pair<const key_type, mapped_type>;
        using reference = value_type &;
        using const_reference = const value_type &;
        using size_type = typename Base::size_type;

        using iterator = typename Base::template SetIterator<false>;
        using const_iterator = typename Base::const_iterator;


        using Base::Base;
        using Base::insert;

        map(const map &m) : Base(m) {}

        map(map &&m) noexcept: Base(std::move(m)) {}

        map &operator=(const map &m) {
            if (this == &m)
                return *this;
            map tmp(m);
            *this = std::move(tmp);
            return *this;
        }

        map &operator=(map &&m) noexcept {
            if (this == &m)
                return *this;
            Base::clear();
            if (Base::fake_root_)
                std::allocator_traits<typename Base::allocator_type_node>::deallocate
                        (Base::node_alloc_, Base::fake_root_, 1);
            Base::size_ = m.size_;
            Base::fake_root_ = m.fake_root_;
            if constexpr(Base::kComparator_moves) {
                Base::comparator_ = std::move(m.comparator_);
            } else {
                Base::comparator_ = m.comparator_;
            }
            Base::alloc_ = std::move(m.alloc_);
            Base::node_alloc_ = std::move(m.node_alloc_);
            m.size_ = 0;
            m.fake_root_ = nullptr;
            return *this;
        }

        ~map() = default;

        /**
        * @brief returns const reference to object mapped to key
        * if no such key exists throws
        */
        const mapped_type &at(const key_type &key) const {
            auto it = Base::find(key);
            if (it == Base::end()) {
                throw std::out_of_range("No element with such key");
            } else {
                return it->second;
            }
        }

        /**
         * @brief returns reference to object mapped to key
         * if no such key exists throws
         */
        mapped_type &at(const key_type &key) {
            auto it = Base::find(key);
            if (it == Base::end()) {
                throw std::out_of_range("No element with such key");
            } else {
                return it->second;
            }
        }

        /**
         * @brief Returns a reference to the value that is
         * mapped to a key equivalent to key,
         * performing an insertion if such key does not already exist.
         */
        mapped_type &operator[](const Key &key) {
            auto it = Base::find(key);
            if (it == Base::end()) {
                return insert(std::make_pair(key, T())).first->second;
            } else {
                return it->second;
            }
        }

        /**
         * @brief Returns a reference to the value that is
         * mapped to a key equivalent to key,
         * performing an insertion if such key does not already exist.
         */
        mapped_type &operator[](Key &&key) {
            auto it = Base::find(key);
            if (it == Base::end()) {
                return insert(std::make_pair(std::move(key), T())).first->second;
            } else {
                return it->second;
            }
        }

        /**
         * @brief attempts to insert key, mapped type pair. returns iterator to element with key and
         * true if insertion took place, false otherwise
         */
        template <class M>
        std::pair<iterator, bool> insert(const key_type &key, M&& obj) {
            ///template and forward instead of 4 different overloads because compiler finds them ambiguous
            ///passing garbage to M thankfully leads to CE. Praise compiler writers
            return insert(std::make_pair(key, std::forward<M>(obj)));
        }
        /**
         * @brief attempts to insert key, mapped type pair. returns iterator to element with key and
         * true if insertion took place, false otherwise
         */
        template <class M>
        std::pair<iterator, bool> insert(key_type &&key, M&& obj) {
            return insert(std::make_pair(std::move(key), std::forward<M>(obj)));
        }

        /**
         * @brief if key already exists changes object mapped to it; otherwise inserts pair\n
         * returns iterator to element in map and true if insertion took place/false if it didn't
         */
        template <class M>
        std::pair<iterator, bool> insert_or_assign(const key_type& key, M&& obj) {
            auto it = Base::find(key);
            if (it == Base::end()) {
                return insert(std::make_pair(key, std::forward<M>(obj)));
            } else {
                it->second = std::forward<M>(obj);
                return std::make_pair(it, false);
            }
        }
        /**
         * @brief if key already exists changes object mapped to it; otherwise inserts pair\n
         * returns iterator to element in map and true if insertion took place/false if it didn't
         */
        template <class M>
        std::pair<iterator, bool> insert_or_assign(key_type&& key, M&& obj) {
            auto it = Base::find(key);
            if (it == Base::end()) {
                return insert(std::make_pair(std::move(key), std::forward<M>(obj)));
            } else {
                it->second = std::forward<M>(obj);
                return std::make_pair(it, false);
            }
        }

        void swap(map& other){
            Base::swap(other);
        }

        void merge(map& other){
            Base::merge(other);
        }

    };

}//namespace s21
#endif //S21_CONTAINERS_S21_MAP_H_