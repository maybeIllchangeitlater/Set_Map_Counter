#ifndef S21_CONTAINERS_S21_SET_H_
#define S21_CONTAINERS_S21_SET_H_

#include <algorithm>
#include <initializer_list>

#include <iterator>
#include <utility>
#include <iostream> //delete later
#include <type_traits>
#include "TreeAllocator.h"
#include "decorator.h"

//template<typename T>
//class SetIterator;
namespace s21{
template<typename T>
struct MyComparator {
    bool operator()(const T &lhs, const T &rhs) const {
        return lhs < rhs;
    }

}; //no real reason, just for fun.



template<typename T, typename Compare = MyComparator<T>, typename Allocator = MyTreeAllocator<T>>
class set {
public:

    struct Node {
        T __key_;
        size_t __height_;
        Node *__parent_;
        Node *__left_;
        Node *__right_;
    };

    using allocator_type = Allocator;
    using allocator_type_node = typename std::allocator_traits<allocator_type>::template rebind_alloc<Node>;
    using key_type = const T;
    using value_type = key_type;
    using reference = const T &;
    using key_compare = Compare;
    using value_compare = key_compare;
    using const_reference = const T &;
    using size_type = size_t;
    class SetIterator {
    public:
        using iterator = SetIterator;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using reference = const T &;
        using pointer = const T *;


        SetIterator() = default;

        explicit SetIterator(Node *n) : n_(n) {}

        virtual ~SetIterator() = default;


        reference operator*() const {
            return n_->__key_;
        }

        /**
         * @brief Starting from leftmost node, check if right node exists. If it does, go to it's leftmost node
         *  if it doesn't go up until you either encounter right(unvisited) node or nullptr
         */
        iterator operator++() {
            if (n_->__right_) {
                n_ = FindLeftmost(n_->__right_);
            } else {
                while (n_->__parent_ && n_->__parent_->__right_ == n_) {
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
                while (n_->__parent_ && n_->__parent_->__left_ == n_) {
                    n_ = n_->__parent_;
                }
                n_ = n_->__parent_;
            }
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
        friend class s21::set<T>;
    };

    using iterator = SetIterator;
    using const_iterator = SetIterator;


    set() : size_(0), root_(nullptr), comparator_(Compare()), alloc_(), node_alloc_(alloc_) {}

    /**
     * @brief Wont compile if alloc type and template alloc are different or if comparator is not copy
     * constructable;\n
     * As per allocator requirements all allocators related by rebind
     * maintain each other's resources, such as memory pools therefore rebound alloc
     * can be constructed through non-rebound alloc
     * @param comp instance of template type
     * @param alloc instance of template type or default constructed template alloc by default
     */
    explicit set(const Compare& comp, const Allocator& alloc = Allocator()):  size_(0),
    root_(nullptr), comparator_(comp), alloc_(alloc), node_alloc_(alloc){}

    /**
     * @brief Wont compile if alloc type and template alloc are different
     * As per allocator requirements all allocators related by rebind
     * maintain each other's resources, such as memory pools therefore rebound alloc
     * can be constructed through non-rebound alloc
     * @param alloc instance of template type
     */
    explicit set(const Allocator& alloc ) :
    size_(0), root_(nullptr), comparator_(Compare()), alloc_(alloc), node_alloc_(alloc){}

    set(std::initializer_list<value_type> init, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) :
    set(comp, alloc){
        for (auto &v: init)
            EmplaceAppend(std::move(v));
    }

    set(std::initializer_list<value_type> init, const Allocator& alloc) : set(init, Compare(), alloc) {}


    /**
     * @brief Copies everything from set s.\n If comparator is non-copy constructible will choose
     * create default constructed copy
     */
    set(const set &s) :
    size_(0), root_(nullptr), comparator_(std::is_copy_constructible_v<Compare> ? s.key_comp() : Compare()),
    alloc_(s.get_allocator()), node_alloc_(alloc_) {
        for (const auto &v: s)
            Add(v);
    }



    set(set &&s)  noexcept  : size_(s.size_), root_(s.root_),
    comparator_(kComparator_moves ? std::move(s.comparator_) : s.comparator_),
    alloc_(std::move(s.alloc_)), node_alloc_(std::move(s.node_alloc_)){
        s.size_ = 0;
        s.root_ = nullptr;
    }

    set &operator=(const set &s) {
        if(this == &s)
            return *this;
        clear();
        comparator_ = std::is_copy_constructible_v<Compare> ? s.key_comp() : Compare();
        alloc_ = s.alloc_;
        node_alloc_ = s.node_alloc_;
        for (const auto &v: s)
            Add(v);
        return *this;
    }

    set &operator=(set &&s)  noexcept {
        if(this == &s)
            return *this;
        clear();
        size_ = s.size_;
        root_ = s.root_;
        comparator_ = kComparator_moves ? std::move(s.comparator_) : s.comparator_;
        alloc_ = std::move(s.alloc_);
        node_alloc_ = std::move(s.node_alloc_);
        s.size_ = 0;
        s.root_ = nullptr;
        return *this;
    }

    ~set(){
        clear();
    };
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
            root_ = Insert(root_, target);
            ++size_;
            return std::make_pair(find(target->__key_), true);
        }
    }
    /**
     * @brief insert element into a tree and returns iterator to it. If node already exists returns false and iterator
     * to existing node
     */
    std::pair<iterator, bool> insert(value_type& value) {
        auto it = find(value);
        if (it != end()) {
            return std::make_pair(it, false);
        } else {
            Node * target = AllocateAndConstruct(value);
            root_ = Insert(root_, target);
            ++size_;
            return std::make_pair(find(value), true);
        }
    }
    /**
     * @brief insert element into a tree and returns iterator to it. If node already exists returns false and iterator
     * to existing node
     */
    std::pair<iterator, bool> insert(value_type&& value){
        auto it = find(value);
        if (it != end()) {
            return std::make_pair(it, false);
        } else {
            Node * target = AllocateAndConstruct(std::move(value));
            root_ = Insert(root_, target);
            ++size_;
            return std::make_pair(find(value), true);
        }
    };
    /**
     * @brief insert elements in range of iterator first to iterator second into a tree
     */
    template<class InputIt>
    void insert(InputIt first, InputIt last){
        if constexpr (std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>) {
            for (InputIt it = first; it != last;++it){
                Append(*it);
            }
        }
    }
    void insert(std::initializer_list<value_type> ilist ){
        for(auto & v: ilist){
            EmplaceAppend(std::move(v));
        }
    }
    /**
     * @brief removes node from a tree. does nothing if it doesnt exist
     */
    void erase(reference value) {
        if (contains(value)){
            root_ = Delete(root_, value);
            --size_;
        }
    }
    /**
     * @brief removes node from a tree. does nothing if it doesnt exist
     */
    void erase(iterator pos) {
        erase(*pos);
    }

    void erase(const_iterator first, const_iterator last){
        while(first != last){
            first = erase(first);
        }
    }
    /**
     * @brief returns iterator to position of node with input value or iterator to end
     */
    iterator find(reference &value) const noexcept {
        return iterator(Search(value));
    }

    bool contains(reference value) const noexcept{
        return Search(value);
    }

    /**
     * @brief swaps contents of two sets
     */
    void swap(set &other) {
        std::swap(root_, other.root_);
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
                auto root_and_extract = other.Extract(other.root_, *one_step_behind);
                other.root_ = root_and_extract.first;
                root_and_extract.second->__left_ = nullptr;
                root_and_extract.second->__right_ = nullptr;
                root_and_extract.second->__parent_ = nullptr;
                root_and_extract.second->__height_ = 1;
                root_ = Insert(root_, root_and_extract.second);
            }
            one_step_behind = it;
        }
        if(one_step_behind != other.end() && !contains(*one_step_behind)){
            auto root_and_extract = other.Extract(other.root_, *one_step_behind);
            other.root_ = root_and_extract.first;
            root_and_extract.second->__left_ = nullptr;
            root_and_extract.second->__right_ = nullptr;
            root_and_extract.second->__parent_ = nullptr;
            root_and_extract.second->__height_ = 1;
            root_ = Insert(root_, root_and_extract.second);
        }
    }

    iterator begin() {
        return iterator(FindLeftmost(root_));
    }

    iterator end() {
        return iterator(nullptr);
    }

    iterator begin() const {
        return const_iterator(FindLeftmost(root_));
    }

    iterator end() const{
        return const_iterator(nullptr);
    }

    const_iterator cbegin() const{
        return const_iterator(FindLeftmost(root_));
    }

    const_iterator cend() const{
        return const_iterator(nullptr);
    }

    bool empty() const noexcept{
        return !size_;
    }

    size_type size() const noexcept{
        return size_;
    }

    void clear() {
        if (root_){
            ClearNodes(root_);
            root_ = nullptr;
        }
        size_ = 0;

    }

    constexpr typename std::enable_if<std::is_copy_constructible<key_compare>::value, key_compare>::type key_comp() const {
        return comparator_;
    }

    constexpr typename std::enable_if<std::is_copy_constructible<value_compare>::value, key_compare>::type value_comp() const {
        return comparator_;
    }
    constexpr allocator_type get_allocator()const noexcept{
        return alloc_;
    }

    bool operator==(const set& rhs) const{
        if (size_ != rhs.size_)
            return false;
        return !std::lexicographical_compare(begin(), end(), rhs.begin(), rhs.end()) &&
        !std::lexicographical_compare(rhs.begin(), rhs.end(), begin(), end());
    }
    bool operator!=(const set& rhs) const{
        if (size_ != rhs.size_)
            return true;
        return !std::lexicographical_compare(begin(), end(), rhs.begin(), rhs.end()) || //perepisat
               !std::lexicographical_compare(rhs.begin(), rhs.end(), begin(), end());
    }
    bool operator<(const set& rhs) const{
        return std::lexicographical_compare(begin(), end(), rhs.begin(), rhs.end());
    }
    bool operator>(const set& rhs) const{
        return std::lexicographical_compare(rhs.begin(), rhs.end(), begin(), end());
    }
    bool operator>=(const set& rhs) const{
        return !std::lexicographical_compare(begin(), end(), rhs.begin(), rhs.end());
    }
    bool operator<=(const set& rhs) const{
        return !std::lexicographical_compare(rhs.begin(), rhs.end(), begin(), end());
    }

protected:
    /**
     * @brief noexcept is not required for comparator.
     * Thankfully AVL balancing doesn't use comparator - we can't "lose" allocated nodes during
     * rebalancing.
     */
    bool SafeCompare(const value_type lhs, const value_type rhs) const{
         try{
             return comparator_(lhs, rhs);
         }catch(...){
             const_cast<set<T, Compare, Allocator>*>(this)->clear();
             throw;
         }
     } //const cast is necessitated by const objects using comparator for find, etc. Alternative is delete this

    template <typename... Args>
    Node* AllocateAndConstruct(Args&&... args){
        try {
            Node *target = std::allocator_traits<allocator_type_node>::allocate(node_alloc_, 1);
            target->__left_ = nullptr;
            target->__right_ = nullptr;
            target->__parent_ = nullptr;
            target->__height_ = 1;
            std::allocator_traits<allocator_type>::construct(alloc_, &(target->__key_), std::forward<Args>(args)...);
            return target;
        } catch (...){
            clear();
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
    Node *Search(reference &value) const noexcept {
        Node *tmp = root_;
        while (tmp) {
            if (SafeCompare(value, tmp->__key_)) {
                tmp = tmp->__left_;
            } else if (SafeCompare(tmp->__key_, value)) {
                tmp = tmp->__right_;
            } else {
                return tmp;
            }
        }
        return tmp;
    }
    /**
     * @brief check for node, create node, insert node.
     */
    void Append(value_type& value){
        if(!contains(value)) {
            Add(value);
        }
    }
    /**
     * @brief check for node, create node, insert node.
     */
    void Append(value_type&& value){
        if(!contains(value)) {
            Node* target = AllocateAndConstruct(std::move(value));
            root_ = Insert(root_, target);
            ++size_;
        }
    }
    /**
     * @brief insert for copy constructor/operator. Doesn't check if node already exists
     *
     */
    void Add(value_type& value){
        Node* target = AllocateAndConstruct(value);
        root_ = Insert(root_, target);
        ++size_;
    }
    /**
     * @brief void return emplace version for initlist insert and constructor
     */
    template <typename... Args>
    void EmplaceAppend(Args&&... args){
        Node * target = AllocateAndConstruct(std::forward<Args>(args)...);
        if(!contains(target->__key_)){
            root_ = Insert(root_, target);
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
        if(!root) {
            return target;
        }
        if (SafeCompare(target->__key_, root->__key_)) {
            root->__left_ = Insert(root->__left_, target);
            root->__left_->__parent_ = root;
        } else if (SafeCompare(root->__key_, target->__key_)) {
            root->__right_ = Insert(root->__right_, target);
            root->__right_->__parent_ = root;
        } else {
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
    Node *Delete(Node *root, key_type &value) {
        if (SafeCompare(value, root->__key_)) {
            root->__left_ = Delete(root->__left_, value);
            if (root->__left_) {
                root->__left_->__parent_ = root;
            }
        } else if (SafeCompare(root->__key_, value)) {
            root->__right_ = Delete(root->__right_, value);
            if (root->__right_) {
                root->__right_->__parent_ = root;
            }
        } else {
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
    std::pair<Node*, Node*> Extract(Node* root, key_type& value, Node* res = nullptr){
        if (SafeCompare(value, root->__key_)) {
            auto extract = Extract(root->__left_, value, res);
            root->__left_ = extract.first;
            res = extract.second;
            if (root->__left_) {
                root->__left_->__parent_ = root;
            }
        } else if (SafeCompare(root->__key_, value)) {
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

    static Node *FindLeftmost(Node *root) {
        return root->__left_ ? FindLeftmost(root->__left_) : root;
    }

    static Node *FindRightmost(Node *root) {
        return root->__right_ ? FindRightmost(root->__right_) : root;
    }

    Node *RebalanceFromLeft(Node *root) {
        if (!root->__left_)
            return root->__right_;
        root->__left_ = RebalanceFromLeft(root->__left_);
        return Balance(root);
    }


private:
    static constexpr const bool kComparator_moves = std::is_nothrow_move_constructible_v<Compare> || !std::is_copy_constructible_v<Compare>;
    size_type size_;
    Node * root_;
    /**
     * @brief if comparator_ throws it's fine, it's not used in tree balancing and wont invalidate tree for clear
     */
    Compare comparator_;
    allocator_type alloc_;
    allocator_type_node node_alloc_;


};

};
#endif //S21_CONTAINERS_S21_SET_H_


///// Tests with throws, especially comp. Move and copy constructors. Add fakeroot and leftmost node for O(1) begin and end