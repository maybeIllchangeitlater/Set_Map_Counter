#ifndef S21_CONTAINERS_S21_SET_H_
#define S21_CONTAINERS_S21_SET_H_

#include <algorithm>
#include <initializer_list>

#include <iterator>
#include <utility>
#include <iostream> //delete later
#include <type_traits>
#include "TreeAllocator.h"
//#include "s21_map.h"

//template<typename T>
//class SetIterator;
namespace s21{

    template<typename T>
    struct MyComparator {
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }

    }; //no real reason, just for fun.
//    template<bool Const>
//    class SetIterator;
//    template <typename T>
//    struct SetIteratorSelector {
//        using type = SetIterator<false>;
//    };
//
//    template <typename T = std::pair<typename T1, typename T2>
//    struct SetIteratorSelector<std::pair<const typename T::first_type, typename T::second_type>> {
//        using type = SetIterator<true>;
//    };




    template<typename T, typename Compare = MyComparator<T>, typename Allocator = MyTreeAllocator<T>>
    class set {
    public:

        using allocator_type = Allocator;
        using key_type = T;
        using value_type = key_type;
        using reference = T &;
        using key_compare = Compare;
        using value_compare = key_compare;
        using const_reference = const T &;
        using size_type = size_t;
        static_assert((std::is_same<typename allocator_type::value_type, value_type>::value),
                      "Allocator value type must be the same as value type");

        struct Node {
            value_type __key_;
            size_type __height_;
            Node *__parent_;
            Node *__left_;
            Node *__right_;
        };

        using allocator_type_node = typename std::allocator_traits<allocator_type>::template rebind_alloc<Node>;
        template< bool Const = false >
        class SetIterator  {
        public:
            using iterator = SetIterator;
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using reference = typename std::conditional_t< Const, T const &, T & >;
            using pointer = typename std::conditional_t< Const, T const *, T * >;
            using value_type = T;


            SetIterator() = delete;

            explicit SetIterator(Node *n) : n_(n) {}

            // Conversion operator for converting SetIterator<false> to SetIterator<true>
            template <bool V, std::enable_if_t<V && !Const, int> = 0>
            explicit operator SetIterator<V>() {
                return SetIterator<V>(n_);
            }

            // Conversion operator for converting SetIterator<true> to SetIterator<false>
            template <bool V, std::enable_if_t<!V && Const, int> = 0>
            explicit operator SetIterator<V>() {
                return SetIterator<V>(n_);
            }

            ~SetIterator() = default;


            reference operator*() const {
                return n_->__key_;
            }


            pointer operator->() const{
                return &(n_->__key_);
            }

            /**
             * @brief Starting from leftmost node, check if right node exists. If it does, go to it's leftmost node
             *  if it doesn't go up until you either encounter right(unvisited) node or nullptr
             */
            iterator operator++() {
                if (n_->__right_) {
                    n_ = FindLeftmost(n_->__right_);
                } else {
                    while (n_->__parent_->__parent_ && n_->__parent_->__right_ == n_) {
                        n_ = n_->__parent_;
                    }
                    n_ = n_->__parent_;
                }
                return *this;
            }

            /**
            * @brief Starting from rightmost node, check if left node exists. If it does, go to it's rightmost node
            *  if it doesn't go up until you either encounter left(unvisited) node or nullptr
            */
            iterator operator--() {
                if (n_->__left_) {
                    n_ = FindRightmost(n_->__left_);
                } else {
                    while (n_->__parent_->__parent_ && n_->__parent_->__left_ == n_) {
                        n_ = n_->__parent_;
                    }
                    n_ = n_->__parent_;
                }
                return *this;
            }

            iterator operator++(int) {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            iterator operator--(int) {
                iterator tmp = *this;
                --(*this);
                return tmp;
            }

            bool operator==(const iterator &rhs) const noexcept {
                return n_ == rhs.n_;
            }

            bool operator!=(const iterator &rhs) const noexcept {
                return n_ != rhs.n_;
            }

        protected:
            Node *n_;
        };
    protected:
        template <typename U>
        struct is_map_pair : std::false_type {};

        template <typename First, typename Second>
        struct is_map_pair<std::pair<const First, Second>> : std::true_type {};
    public:
        using iterator = typename std::conditional<is_map_pair<T>::value, SetIterator<false>, SetIterator<true>>::type;
        using const_iterator = SetIterator<true>;





        set() : size_(0), comparator_(Compare()), alloc_(), node_alloc_(alloc_), fake_root_(std::allocator_traits<allocator_type_node>::allocate(node_alloc_, 1)) {
            InitNode(fake_root_);
        }

        /**
         * @brief Wont compile if alloc type and template alloc are different or if comparator is not copy
         * constructable;\n
         * As per allocator requirements all allocators related by rebind
         * maintain each other's resources, such as memory pools therefore rebound alloc
         * can be constructed through non-rebound alloc
         * @param comp instance of template type
         * @param alloc instance of template type or default constructed template alloc by default
         */
        explicit set(const Compare& comp, const Allocator& alloc = Allocator()):  size_(0), comparator_(comp), alloc_(alloc), node_alloc_(alloc),
                                                                                  fake_root_(std::allocator_traits<allocator_type_node>::allocate(node_alloc_, 1)){
            InitNode(fake_root_);
        }

        /**
         * @brief Wont compile if alloc type and template alloc are different
         * As per allocator requirements all allocators related by rebind
         * maintain each other's resources, such as memory pools therefore rebound alloc
         * can be constructed through non-rebound alloc
         * @param alloc instance of template type
         */
        explicit set(const Allocator& alloc) :
                size_(0), comparator_(Compare()), alloc_(alloc), node_alloc_(alloc), fake_root_(std::allocator_traits<allocator_type_node>::allocate(node_alloc_, 1)){
            InitNode(fake_root_);
        }

        set(std::initializer_list<value_type> init, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) :
                set(comp, alloc){
            insert(init);
        }

        set(std::initializer_list<value_type> init, const Allocator& alloc) : set(init, Compare(), alloc) {}


        /**
         * @brief Copies from set s.\n If comparator is non-copy constructible will
         * create default constructed copy
         */

        set(const set &s) : set(s, std::is_copy_constructible<Compare>()) {}


        /**
         * @brief Moves from set s.\n If comparator is nothrow movable or non-copy constructible will
         * copy
         */
        set(set&& s) noexcept : set(std::move(s), kComparator_moves){}


        /**
         * @brief if comp or T has no copy constructor wont compile
         */
        set &operator=(const set &s) {
            if(this == &s)
                return *this;
            set tmp(s);
            *this = std::move(tmp);
            return *this;
        }

        set &operator=(set &&s)  noexcept {
            if(this == &s)
                return *this;
            clear();
            if(fake_root_)
                std::allocator_traits<allocator_type_node>::deallocate(node_alloc_, fake_root_, 1);
            size_ = s.size_;
            fake_root_ = s.fake_root_;
            if constexpr(kComparator_moves){
                comparator_ = std::move(s.comparator_);
            }else{
                comparator_ = s.comparator_;
            }
            alloc_ = std::move(s.alloc_);
            node_alloc_ = std::move(s.node_alloc_);
            s.size_ = 0;
            s.fake_root_ = nullptr;
            return *this;
        }

        ~set(){
            clear();
            if(fake_root_) {
                std::allocator_traits<allocator_type_node>::deallocate(node_alloc_, fake_root_, 1);
                fake_root_ = nullptr;
            }
        }
        /**
         * @brief  Inserts a new element into the container constructed in-place with the given args if there is no element with the key in the container.\n
         * Careful use of emplace allows the new element to be constructed while avoiding unnecessary copy or move operations. The constructor \n
         *   of the new element is called with exactly the same arguments as supplied to emplace, forwarded via std::forward<Args>(args).... \n
         *  The element may be constructed even if there already is an element with the key in the container, in which case the newly constructed \n
         *   element will be destroyed immediately.\n
         *   tldr: worse than insert if element with key exists in tree, better if it doesn't
         */
        template<typename ... Args >
        std::pair<iterator, bool> emplace(Args&&... args){
            Node * target = AllocateAndConstruct(std::forward<Args>(args)...);
            auto it = find(target->__key_);
            if (it != end()) {
                DestructAndDeallocate(target);
                return std::make_pair(it, false);
            } else{
                fake_root_->__left_ = Insert(fake_root_->__left_, target);
                ++size_;
                return std::make_pair(iterator(target), true);
            }
        }
        /**
         * @brief insert element into a tree and returns iterator to it. If node already exists returns false and iterator
         * to existing node
         */
        std::pair<iterator, bool> insert(const value_type& value) {
            auto it = find(value);
            if (it != end()) {
                return std::make_pair(it, false);
            } else {
                Node * target = AllocateAndConstruct(value);
                fake_root_->__left_ = Insert(fake_root_->__left_, target);
                ++size_;
                return std::make_pair(iterator(target), true);
            }
        }
        /**
         * @brief insert element into a tree and returns iterator to it. If node already exists returns false and iterator
         * to existing node
         */
        std::pair<iterator, bool> insert(value_type && value){
            auto it = find(value);
            if (it != end()) {
                return std::make_pair(it, false);
            } else {
                Node * target = AllocateAndConstruct(std::move(value));
                fake_root_->__left_ = Insert(fake_root_->__left_, target);
                ++size_;
                return std::make_pair(iterator(target), true);
            }
        }
        /**
         * @brief insert elements in range of iterator first to iterator second into a tree
         */
        template<typename InputIt>
        void insert(InputIt first, InputIt last){
            if constexpr (std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>) {
                for (InputIt it = first; it != last;++it){
                    Append(*it);
                }
            }
        }
        /**
         * @brief not just reusing iterator insert because ilist is unlikely to contain dupes (unless testing) and we
         * can freely move from ilist
         */
        void insert(std::initializer_list<value_type> ilist){
            for(auto & v: ilist){
                EmplaceAppend(std::move(v));
            }
        }
        /**
         * @brief removes node from the tree. does nothing if it doesnt exist
         */
        void erase(const key_type& value) {
                fake_root_->__left_ = Delete(fake_root_->__left_, value);
                if(fake_root_->__left_)fake_root_->__left_->__parent_ = fake_root_;
        }
        /**
         * @brief removes node from the tree. does nothing if it doesnt exist
         */
        void erase(iterator pos) {
            erase(*pos);
        }
        /**
         * erase elements from first to last(not including).
         * if first is after last behaviour is undefined
         */
        void erase(const_iterator first, const_iterator last){
            while(first != last){
                auto it = first;
                ++first;
                erase(it);
            }
        }
        /**
         * @brief returns const iterator to position of node with input value or past-end iterator
         */
        const_iterator find(const key_type &value) const noexcept {
            return const_iterator(Search(value));
        }
        /**
        * @brief returns iterator to position of node with input value or past-end iterator
        */
        iterator find(const value_type &value) noexcept {
            return iterator(Search(value));
        }

        bool contains(const key_type &value) const noexcept{
            return const_iterator(Search(value)) != end();
        }
        /**
         * @brief Returns an iterator pointing to the first element that is not less than key
         */
        iterator lower_bound(const key_type& key) const noexcept{
            Node* tmp = fake_root_->__left_;
            Node* result = nullptr;
            while (tmp){
                if (!WrappedCompare(tmp->__key_, key)){
                    result = tmp;
                    tmp = tmp->__left_;
                }
                else {
                    tmp = tmp->__right_;
                }
            }
            return iterator(result);
        }
        /**
         * @brief Returns an iterator pointing to the first element that is greater than key
         */
        iterator upper_bound(const key_type& key) const noexcept {
            Node* tmp = fake_root_->__left_;
            Node* result = nullptr;
            while (tmp){
                if (WrappedCompare(key, tmp->__key_)){
                    result = tmp;
                    tmp = tmp->__left_;
                }
                else {
                    tmp = tmp->__right_;
                }
            }
            return iterator(result);
        }

        /**
         * @brief swaps contents of two sets
         */
        void swap(set &other) {
            std::swap(fake_root_, other.fake_root_);
            std::swap(size_, other.size_);
            std::swap(comparator_, other.comparator_);
            std::swap(alloc_, other.alloc_);
            std::swap(node_alloc_, other.node_alloc_);
        }

        /**
         * @brief splices Nodes from other set to current set
         */
        void merge(set& other){
            auto one_step_behind = other.end();
            for(auto it = other.begin(); it != other.end(); ++it){
                if(one_step_behind != other.end() && !contains(*one_step_behind)){
                    auto root_and_extract = other.Extract(other.fake_root_->__left_, *one_step_behind);
                    other.fake_root_->__left_ = root_and_extract.first;
                    other.fake_root_->__left_->__parent_ = other.fake_root_;
                    InitNode(root_and_extract.second);
                    fake_root_->__left_ = Insert(fake_root_->__left_, root_and_extract.second);
                    ++size_;
                }
                one_step_behind = it;
            }
            if(one_step_behind != other.end() && !contains(*one_step_behind)){
                auto root_and_extract = other.Extract(other.fake_root_->__left_, *one_step_behind);
                other.fake_root_->__left_ = root_and_extract.first;
                other.fake_root_->__left_->__parent_ = other.fake_root_;
                InitNode(root_and_extract.second);
                fake_root_->__left_ = Insert(fake_root_->__left_, root_and_extract.second);
                ++size_;
            }
        }

        iterator begin(){
            return iterator(FindLeftmost(fake_root_->__left_));
        }
        iterator end(){
            return iterator(fake_root_);
        }
        const_iterator begin() const {
            return const_iterator(FindLeftmost(fake_root_->__left_));
        }

        const_iterator end() const{
            return const_iterator(fake_root_);
        }

        const_iterator cbegin() const{
            return const_iterator(FindLeftmost(fake_root_->__left_));
        }

        const_iterator cend() const{
            return const_iterator(fake_root_);
        }

        bool empty() const noexcept{
            return size_ == 0;
        }

        size_type size() const noexcept{
            return size_;
        }

        void clear() {
            if(fake_root_) {
                if (fake_root_->__left_) {
                    ClearNodes(fake_root_->__left_);
                }
                fake_root_->__left_ = nullptr;
                size_ = 0;
            }
        }
        constexpr key_compare key_comp() const {
            return comparator_;
        }
        constexpr key_compare value_comp() const {
            return comparator_;
        }

        constexpr allocator_type get_allocator()const noexcept{
            return alloc_;
        }

        bool operator==(const set& rhs) const{
            if (size_ != rhs.size_)
                return false;
            auto it1 = begin();
            auto it2 = rhs.begin();
            while (it1 != end() && it2 != rhs.end()) {
                if (*it1 != *it2) {
                    return false;
                }
                ++it1;
                ++it2;
            }
            return true;
        }

        bool operator!=(const set& rhs) const{
            return !(*this = rhs);
        }
        bool operator<(const set& rhs) const{
            return std::lexicographical_compare(begin(), end(), rhs.begin(), rhs.end());
        }
        bool operator>(const set& rhs) const{
            return rhs < *this;
        }
        bool operator>=(const set& rhs) const{
            return !(*this < rhs);
        }
        bool operator<=(const set& rhs) const{
            return !(rhs < *this);
        }

    protected:

        set(const set &s, std::true_type) :
                size_(0), comparator_(s.comparator_), alloc_(s.get_allocator()), node_alloc_(alloc_), fake_root_(std::allocator_traits<allocator_type_node>::allocate(node_alloc_, 1)) {
            InitNode(fake_root_);
            for (const auto &v: s)
                Add(v);
        }

        set(const set& s, std::false_type) :
                size_(0), comparator_(Compare()), alloc_(s.get_allocator()), node_alloc_(alloc_), fake_root_(std::allocator_traits<allocator_type_node>::allocate(node_alloc_, 1)) { //add rootparent
            InitNode(fake_root_);
            for (const auto &v: s)
                Add(v);
        }

        set(set&& s, std::true_type) noexcept : size_(s.size_), comparator_(std::move(s.comparator_)), alloc_(std::move(s.alloc_)), node_alloc_(std::move(s.alloc_)), fake_root_(std::move(s.fake_root_)){
            s.fake_root_ = nullptr;
            s.size_ = 0;
        }

        set(set&& s, std::false_type) noexcept : size_(s.size_), comparator_(s.comparator_), alloc_(std::move(s.alloc_)), node_alloc_(std::move(s.alloc_)), fake_root_(std::move(s.fake_root_)){
            s.fake_root_ = nullptr;
            s.size_ = 0;
        }

        /**
         * @brief purely for inheritance
         */
        bool WrappedCompare(const value_type& lhs, const value_type& rhs) const{
            if constexpr(is_map_pair<T>::value)
                return comparator_(lhs.first, rhs.first);
            else
                return comparator_(lhs, rhs);
        }

        template <typename... Args>
        Node* AllocateAndConstruct(Args&&... args){
            Node *target = std::allocator_traits<allocator_type_node>::allocate(node_alloc_, 1);
            InitNode(target);
            try {
                std::allocator_traits<allocator_type>::construct(alloc_, &(target->__key_), std::forward<Args>(args)...);
                return target;
            } catch (...){
                DestructAndDeallocate(target);
                throw;
            }
        }

        void DestructAndDeallocate(Node* target){
            std::allocator_traits<allocator_type>::destroy(alloc_, &(target->__key_));
            std::allocator_traits<allocator_type_node>::deallocate(node_alloc_, target, 1);
        }
        /**
         * @brief recursively clears everything to the right and to the left from node before clearing the node
         */
        void ClearNodes(Node* root){
            if(root->__left_) {
                if(!root->__left_->__left_&& !root->__left_->__right_){
                    DestructAndDeallocate(root->__left_);
                }
                else ClearNodes(root->__left_);
            }
            if(root->__right_) {
                if(!root->__right_->__left_ && !root->__right_->__right_){
                    DestructAndDeallocate(root->__right_);
                }
                else ClearNodes(root->__right_);
            }

            DestructAndDeallocate(root);
        }
        /**
         * @brief preforms binary search for Node of value
         */
         Node *Search(const key_type &value) const noexcept {
            Node *tmp = fake_root_->__left_;
            while (tmp) {
                if (WrappedCompare(value, tmp->__key_)) {
                    tmp = tmp->__left_;
                } else if (WrappedCompare(tmp->__key_, value)) {
                    tmp = tmp->__right_;
                } else {
                    return tmp;
                }
            }
            return fake_root_;
        }
        /**
         * @brief check for node, create node, insert node.
         */
        void Append(const_reference value){
            if(!contains(value)) {
                Add(value);
            }
        }

        /**
         * @brief insert for copy constructor/operator. Doesn't check if node already exists
         *
         */
        void Add(const_reference  value){
            Node* target = AllocateAndConstruct(value);
            fake_root_->__left_ = Insert(fake_root_->__left_, target);
            ++size_;
        }
        /**
         * @brief void return emplace version for initlist insert
         */
        template <typename... Args>
        void EmplaceAppend(Args&&... args){
            Node * target = AllocateAndConstruct(std::forward<Args>(args)...);
            if(!contains(target->__key_)){
                fake_root_->__left_ = Insert(fake_root_->__left_, target);
                ++size_;
            }else{
                 DestructAndDeallocate(target);
            }
        }
        /**
        * @brief Recursively searches for place to insert leaf and balances tree from bottoms up
        * If node already exist will do nothing but attempting few rebalances; Prefereably find first;
        */
        Node *Insert(Node *root, Node *target) {
            if(!root){
                //if its first node
                if(!fake_root_->__left_) target->__parent_ = fake_root_;
                //if node is a leaf
                return target;
            }
            if (WrappedCompare(target->__key_, root->__key_)) {
                root->__left_ = Insert(root->__left_, target);
                root->__left_->__parent_ = root;
            } else if (WrappedCompare(root->__key_, target->__key_)) {
                root->__right_ = Insert(root->__right_, target);
                root->__right_->__parent_ = root;
            } else {
                //already exists
                return root;
            }
            return Balance(root);
        }

        /**
        * @brief Recursively searches for node to delete. if it's a leaf or a node with 1 child
        * node can be deleted and 'replaced' by child
        * if it's a node with 2 children, node is 'replaced' by leftmost node from right child
        * (or you can change it to rightmost from left child, doesn't matter)
        */
        Node *Delete(Node *root, const key_type &value) {
            if (WrappedCompare(value, root->__key_)) {
                root->__left_ = Delete(root->__left_, value);
                if (root->__left_) {
                    root->__left_->__parent_ = root;
                }
            } else if (WrappedCompare(root->__key_, value)) {
                root->__right_ = Delete(root->__right_, value);
                if (root->__right_) {
                    root->__right_->__parent_ = root;
                }
            } else {
                --size_;
                Node * left = root->__left_;
                Node * right = root->__right_;
                DestructAndDeallocate(root);
                if(!right) {
                    if(left) left->__parent_ = nullptr;
                    return left;
                }
                Node *new_root = FindLeftmost(right);
                new_root->__parent_ = nullptr;
                new_root->__right_ = RebalanceFromLeft(right);
                if (new_root->__right_) new_root->__right_->__parent_ = new_root;
                new_root->__left_ = left;
                if (new_root->__left_) new_root->__left_->__parent_ = new_root;
                return Balance(new_root);
            }

            return Balance(root);
        }
        /**
         * @brief extracts Node from the tree, unlinking it but not deleting it
         */
        std::pair<Node*, Node*> Extract(Node* root, const key_type& value, Node* res = nullptr){
            if (WrappedCompare(value, root->__key_)) {
                auto extract = Extract(root->__left_, value, res);
                root->__left_ = extract.first;
                res = extract.second;
                if (root->__left_) {
                    root->__left_->__parent_ = root;
                }
            } else if (WrappedCompare(root->__key_, value)) {
                auto extract = Extract(root->__right_, value, res);
                root->__right_ = extract.first;
                res = extract.second;
                if (root->__right_) {
                    root->__right_->__parent_ = root;
                }
            } else {
                --size_;
                Node * left = root->__left_;
                Node * right = root->__right_;
                if(!right) {
                    if(left) left->__parent_ = nullptr;
                    return std::make_pair(left, root);
                }
                Node *new_root = FindLeftmost(right);
                if(new_root->__right_) new_root->__right_->__parent_ = new_root->__parent_;
                if(new_root->__left_) new_root->__left_->__parent_ = new_root->__parent_;
                new_root->__parent_ = nullptr;
                new_root->__right_ = RebalanceFromLeft(right);
                if (new_root->__right_) new_root->__right_->__parent_ = new_root;
                new_root->__left_ = left;
                if (new_root->__left_) new_root->__left_->__parent_ = new_root;
                return std::make_pair(Balance(new_root), root);
            }

            return std::make_pair(Balance(root), res);
        }

        /**
        * @brief bf(x) > 1 means tree leans right and needs to be balanced, if righ subtree also leans right its fine
        * otherwise you need to balance it first. same thing for < 1, balancing is done recursively so just think
        * of it as small 3 node trees.
        */
        Node *Balance(Node *root) noexcept {
            FixHeight(root);
            if (BalanceFactor(root) > 1) {
                if (BalanceFactor(root->__right_) < 0) {
                    root->__right_ = RotateRight(root->__right_);
                }
                return RotateLeft(root);
            }
            if (BalanceFactor(root) < -1) {
                if (BalanceFactor(root->__left_) > 0) {
                    root->__left_ = RotateLeft(root->__left_);
                }
                return RotateRight(root);
            }
            return root;
        }

        /**
        * @brief think of it as 3 nodes tree. Take right child, make it root, root goes to left and
        * left goes to right
        *
        */
        Node *RotateLeft(Node *root) noexcept {
            Node *tmp = root->__right_;
            root->__right_ = tmp->__left_;
            tmp->__parent_ = root->__parent_;
            if (tmp->__left_) tmp->__left_->__parent_ = root;
            tmp->__left_ = root;
            root->__parent_ = tmp;
            FixHeight(root);
            FixHeight(tmp);
            return tmp;
        }

        /**
        * @brief think of it as 3 nodes tree. Take left child, make it root, root goes to right and
        * right goes to left
        *
        */
        Node *RotateRight(Node *root) noexcept {
            Node *tmp = root->__left_;
            root->__left_ = tmp->__right_;
            tmp->__parent_ = root->__parent_;
            if (tmp->__right_) tmp->__right_->__parent_ = root;
            tmp->__right_ = root;
            root->__parent_ = tmp;
            FixHeight(root);
            FixHeight(tmp);
            return tmp;

        }

        void FixHeight(Node *root) noexcept {
            size_type left_h = Height(root->__left_);
            size_type right_h = Height(root->__right_);
            root->__height_ = std::max(left_h, right_h) + 1;
        }

        int BalanceFactor(Node *root) const noexcept {
            return Height(root->__right_) - Height(root->__left_);
        }

        size_type Height(Node *root) const noexcept {
            return root ? root->__height_ : 0;
        }

        static Node *FindLeftmost(Node *root) noexcept { //dereferencing nullptr is not an exception (not going to happen I hope)
            return root->__left_ ? FindLeftmost(root->__left_) : root;
        }

        static Node *FindRightmost(Node *root)  noexcept{
            return root->__right_ ? FindRightmost(root->__right_) : root;
        }

        Node *RebalanceFromLeft(Node *root) noexcept{
            if (!root->__left_)
                return root->__right_;
            root->__left_ = RebalanceFromLeft(root->__left_);
            return Balance(root);
        }

        /**
         * @brief essentially a crutch to avoid 9000 static casts from node noT to node T in code
         * static casts are still probably preferable
         */
        void InitNode(Node* root) noexcept{
            root->__left_ = nullptr;
            root->__right_ = nullptr;
            root->__parent_ = nullptr;
            root->__height_ = 1;
        }


    protected:
        static constexpr const std::conditional_t<
                std::is_nothrow_move_constructible<Compare>::value || !std::is_copy_constructible<Compare>::value,
                std::true_type,
                std::false_type
        > kComparator_moves{};
        size_type size_;
        /**
         * @brief if comparator_ throws it's fine, it's not used in tree balancing and wont invalidate tree for clear
         */
        Compare comparator_;
        allocator_type alloc_;
        allocator_type_node node_alloc_;
        Node * fake_root_;

    };

} //namespace s21
///there is a nuance. set<const T, U> will work like a map without some map functional. On the other hand, wtf is set<const T, U>
#endif //S21_CONTAINERS_S21_SET_H_