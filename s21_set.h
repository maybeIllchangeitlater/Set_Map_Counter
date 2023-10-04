#ifndef S21_CONTAINERS_S21_SET_H_
#define S21_CONTAINERS_S21_SET_H_

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <iterator>
#include <utility>
#include <iostream> //delete later
#include <queue>
#include <type_traits>

//template<typename T>
//class SetIterator;
namespace s21{
template<typename T>
struct MyComparator {
    bool operator()(const T &lhs, const T &rhs) const {
        return lhs < rhs;
    }
}; //no real reason, just for fun.


//
//    template<typename Iterator>
//    struct iterator_traits {
//        using value_type = typename Iterator::value_type;
//        using difference_type = typename Iterator::difference_type;
//        using pointer = typename Iterator::pointer;
//        using reference = typename Iterator::reference;
//        using iterator_category = typename Iterator::iterator_category;
//    };
//
//    template<typename T>
//    struct iterator_traits<T *> {
//        using value_type = T;
//        using difference_type = ptrdiff_t;
//        using pointer = const T*;
//        using reference = const T&;
//        using iterator_category = std::bidirectional_iterator_tag;
//    };
//
//    template<typename T>
//    struct iterator_traits<const T *> {
//        using value_type = T;
//        using difference_type = ptrdiff_t;
//        using pointer = const T *;
//        using reference = const T &;
//        using iterator_category = random_access_iterator_tag;
//    };
//static_assert("boba" == "aboba");
template<typename T>
class MyTreeAllocator final{
public:
    using value_type = T;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using is_always_equal = std::true_type;
    using propagate_on_container_swap = std::false_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_copy_assignment = std::false_type;

//    constexpr typename std::enable_if_t<std::is_class<T>::value>
    ~MyTreeAllocator(){
        ::operator delete(pool_);
    }

    /**
     * @brief rebind T to node T
     */
    template <typename U>
    struct rebind
    {
        using other = MyTreeAllocator<U>;
    };

    /**
     * @brief gets called only for Node
     * reallocs 1.5 if no allocate memory left otherwise takes empty(left) node, puts it to the right
     * and returns it
     * make sure n is 1
     * prehaps smarter move to make use of n?
     */

    [[nodiscard]]static pointer allocate(const size_type n){
        std::cout << "ABOBA GIBS MEMORY" << std::endl;
        if(!reusable_ || !reusable_->left)
            pool_ ? realloc_pool(n) : init_pool(n);

        auto ret = reusable_->left;
        reusable_->left = reusable_->left->left;
        ret->left = nullptr;
        ret->parent = nullptr;
        ret->right = nullptr;
        return ret;
    }

    /**
     * @brief doesn't dealloc anything. Only called for Node T
     * moves node to the left from reusable so it can be reused again
     * implies call to destroy for T beforehands
     */
    static void deallocate(const pointer ptr, const size_type n){
        std::cout << "ABOBA REUSES" << std::endl;
        ptr->left = reusable_->left;
        reusable_->left = ptr;
    }

    /**
     * @brief placement new. Only gets called for T. No logic here, can be replaced
     * with default implementation from allocator_traits
     */
    template <typename U, typename... Args>
    static void construct(U* ptr, Args&&... args){
        std::cout << "ABOBA CONSTRUCTS" << std::endl;;
        new (ptr) U(std::forward<Args>(args)...);
    }

    /**
     * @brief can be replaced with default allocator_traits implementation
     * Only gets called for T
     */
    static void destroy(const pointer ptr){
        std::cout << "ABOBA YEETS " << *ptr << std::endl;
        ptr->~T();
    }


//    bool operator==(const MyTreeAllocator &) const { return true; } //questionalbe, allocators are not stateless
//    bool operator!=(const MyTreeAllocator &) const { return false; }

    static pointer address(reference r) { return &r; } //very questionable
    static const_pointer address(const_reference s) { return &s; }

private:
    static void init_pool(const size_type n){
        std::cout << "ABOBA INITS MEMORY" << std::endl;
        pool_ = static_cast<value_type *>(::operator new(sizeof(value_type) * allocated_ * n));
        auto temp_pool = pool_;
        temp_pool += allocated_;
        reusable_ = temp_pool++;
        for(int i = 1; i < allocated_; ++i){
            auto temp_node = reusable_->left;
            reusable_->left = temp_pool++;
            reusable_->left->left = temp_node;
        }
    }

    static void realloc_pool(const size_type n){
        std::cout << "ABOBA CREATE NEW POOL" << std::endl;
        auto new_pool = static_cast<value_type *>(::operator new(sizeof(value_type) * allocated_ * kAlloc_factor_ * n));
        std::memcpy(new_pool, pool_, allocated_);
        ::operator delete(pool_);
        allocated_ *= kAlloc_factor_;
        auto temp_pool = pool_;
        reusable_ = temp_pool++;
        for(int i = 1; i < allocated_; ++i){
            auto temp_node = reusable_->left;
            reusable_->left = temp_pool++;
            reusable_->left->left = temp_node;
        }

    }

    constexpr static const float kAlloc_factor_ = 1.5;
    inline static size_type allocated_ = 10;
    inline static pointer pool_ = nullptr;
    inline static pointer reusable_ = nullptr;
};


    template<typename T, typename Compare = MyComparator<T>, typename Alloc = MyTreeAllocator<T>>
class set {
public:

    struct Node {
        T key;
        size_t height = 1;
        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;
    };
    using allocator_type = Alloc;
    using NodeAlloc = typename std::allocator_traits<allocator_type>::template rebind_alloc<Node>;
    using key_type = const T;
    using value_type = const T;
    using reference = value_type &;
    using const_reference = const value_type &;
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
            return n_->key;
        }

        /**
         * @brief Starting from leftmost node, check if right node exists. If it does, go to it's leftmost node
         *  if it doesn't go up until you either encounter right(unvisited) node or nullptr
         */
        iterator operator++() {
            if (n_->right) {
                n_ = FindLeftmost(n_->right);
            } else {
                while (n_->parent && n_->parent->right == n_) {
                    n_ = n_->parent;
                }
                n_ = n_->parent;
            }
            return *this;
        }

        /**
        * @brief Starting from rightmost node, check if left node exists. If it does, go to it's rightmost node
        *  if it doesn't go up until you either encounter left(unvisited) node or nullptr
        */
        iterator operator--() {
            if (n_->left) {
                n_ = FindRightmost(n_->left);
            } else {
                while (n_->parent && n_->parent->left == n_) {
                    n_ = n_->parent;
                }
                n_ = n_->parent;
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
    };

    using iterator = SetIterator;
    using const_iterator = SetIterator;

    set() noexcept : size_(0), root_(nullptr) {};

    set(std::initializer_list<value_type> const &items) : set() {
        for (const auto &v: items)
            Append(v);
    }

    set(const set &s) : set() {
        for (const auto &v: s)
            Append(v);
    }

    set(set &&s)  noexcept : set() {
        size_ = std::exchange(s.size_, size_);
        root_ = std::exchange(s.root_, root_);
//        alloc_ = std::exchange(s.alloc_, alloc_);
//        node_alloc_ = std::exchange(s.node_alloc_, node_alloc_);
    }

    set &operator=(const set &s) {
        if(this == s)
            return *this;
        clear();
        for (const auto &v: s)
            Append(v);
        return *this;
    }

    set &operator=(set &&s)  noexcept {
        if(this == s)
            return *this;
        clear();
        size_ = std::exchange(s.size_, size_);
        root_ = std::exchange(s.root_, root_);
//        alloc_ = std::exchange(s.alloc_, alloc_);
//        node_alloc_ = std::exchange(s.node_alloc_, node_alloc_);
        return *this;
    }

    virtual ~set(){
        clear();
    };
    /**
     * @brief insert element into a tree and returns iterator to it. If node already exists returns false and iterator
     * to existing node
     */
    std::pair<iterator, bool> insert(reference value) {
        auto it = find(value);
        if (it != end()) {
            return std::make_pair(it, false);
        } else {
            Node* target = allocate_and_construct(value);
            root_ = Insert(root_, target);
            ++size_;
            return std::make_pair(find(value), true);
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
    }

    /**
     * @brief splices Nodes from other set to current set
     */
//    void merge(set& other){
//        for(auto it = other.begin(); it != other.end(); ++it){
//            Node* tmp = search(*it);
//            if(tmp == end()){
//                tmp = other.search(*it);
//            }
//
//        }
//        for(auto it = ++other.begin(); it != end(); ++it) {
//            Node<T>* n = other.search(*(--it));
//            if(n){
//                root_ = Insert(root_, n); // recursive pass current iterator and insert previous into root?
//            }
//        }
//    }

    iterator begin() {
        return iterator(FindLeftmost(root_));
    }

    iterator end() {
        return iterator(nullptr);
    }

    bool empty() const noexcept{
        return !size_;
    }

    const size_type &size() const noexcept{
        return size_;
    }

    void clear() {
        if(root_) ClearNodes(root_);
        root_ = nullptr;
        size_ = 0;
    }

protected:
    Node* allocate_and_construct(reference value){
        try {
//            Node *target = std::allocator_traits<NodeAlloc>::allocate(node_alloc_, 1);
            Node* target = NodeAlloc::allocate(1);
            allocator_type ::construct(&(target->key), value);
//            Node * target = node_alloc_.allocate(1);
//            std::allocator_traits<allocator_type>::construct(alloc_, &(target->key), value);
            return target;
        } catch (...){
            clear();
            throw;
        }
    }

    void destruct_and_deallocate(Node* target){
        allocator_type::destroy(&(target->key));
        NodeAlloc::deallocate(target, 1);
//        std::allocator_traits<allocator_type>::destroy(alloc_, &(target->key));
//        std::allocator_traits<NodeAlloc>::deallocate(node_alloc_, target, 1);
    }
    /**
     * @brief recursively clears everything to the right and to the left from node before clearing the node
     */
    void ClearNodes(Node* root){
        if(root->left) {
            if(!root->left->left&& !root->left->right){
                destruct_and_deallocate(root->left);
            }
            else ClearNodes(root->left);
        }
        if(root->right) {
            if(!root->right->left && !root->right->right){
                destruct_and_deallocate(root->right);
            }
            else ClearNodes(root->right);
        }

        destruct_and_deallocate(root);
    }
    /**
     * @brief preforms binary search for Node of value
     */
    Node *Search(reference &value) const noexcept {
        Node *tmp = root_;
        while (tmp) {
            if (comparator_(value, tmp->key)) {
                tmp = tmp->left;
            } else if (comparator_(tmp->key, value)) {
                tmp = tmp->right;
            } else {
                return tmp;
            }
        }
        return tmp;
    }
    /**
     * @brief check for node, create node, insert node.
     */
    void Append(reference value){
        if(!contains(value)) {
            Node* target = allocate_and_construct(value);
            root_ = Insert(root_, target);
            ++size_;
        }
    }
    /**
    * @brief Recursively searches for place to insert leaf and balances tree from bottoms up
    * If node already exist will do nothing but attempting few rebalances; Prefereably find first;
    */
    Node *Insert(Node *root, Node *target) {
        if (!root) {
            return target;
        }
        if (comparator_(target->key, root->key)) {
            root->left = Insert(root->left, target);
            root->left->parent = root;
        } else if (comparator_(root->key, target->key)) {
            root->right = Insert(root->right, target);
            root->right->parent = root;
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
    Node *Delete(Node *root, reference &value) {
        if (!root) {
            return nullptr;
        }
        if (comparator_(value, root->key)) {
            root->left = Delete(root->left, value);
            if (root->left) {
                root->left->parent = root;
            }
        } else if (comparator_(root->key, value)) {
            root->right = Delete(root->right, value);
            if (root->right) {
                root->right->parent = root;
            }
        } else {
            if (!root->left || !root->right) {
                Node *needs_father_figure = root->left ? root->left : root->right;
                if (needs_father_figure) {
                    needs_father_figure->parent = nullptr;
                }
                destruct_and_deallocate(root);
                return needs_father_figure;
            }
            Node *new_root = FindLeftmost(root->right);
            new_root->parent = nullptr;
            new_root->right = RebalanceFromLeft(root->right);
            if (new_root->right) new_root->right->parent = new_root;
            new_root->left = root->left;
            if (new_root->right) new_root->right->parent = new_root;
            destruct_and_deallocate(root);
            return Balance(new_root);
        }

        return Balance(root);
    }

    /**
    * @brief bf(x) > 1 means tree leans right and needs to be balanced, if righ subtree also leans right its fine
    * otherwise you need to balance it first. same thing for < 1, balancing is done recursively so just think
    * of it as small 3 node trees.
    */
    Node *Balance(Node *root) noexcept {
        FixHeight(root);
        if (BalanceFactor(root) > 1) {
            if (BalanceFactor(root->right) < 0) {
                root->right = RotateRight(root->right);
            }
            return RotateLeft(root);
        }
        if (BalanceFactor(root) < -1) {
            if (BalanceFactor(root->left) > 0) {
                root->left = RotateLeft(root->left);
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
        Node *tmp = root->right;
        root->right = tmp->left;
        tmp->parent = root->parent;
        if (tmp->left) tmp->left->parent = root;
        tmp->left = root;
        root->parent = tmp;
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
        Node *tmp = root->left;
        root->left = tmp->right;
        tmp->parent = root->parent;
        if (tmp->right) tmp->right->parent = root;
        tmp->right = root;
        root->parent = tmp;
        FixHeight(root);
        FixHeight(tmp);
        return tmp;

    }

    void FixHeight(Node *root) noexcept {
        size_type left_h = Height(root->left);
        size_type right_h = Height(root->right);
        root->height = std::max(left_h, right_h) + 1;
    }

    int BalanceFactor(Node *root) const noexcept {
        return Height(root->right) - Height(root->left);
    }

    size_type Height(Node *root) const noexcept {
        return root ? root->height : 0;
    }

    static Node *FindLeftmost(Node *root) {
        return root->left ? FindLeftmost(root->left) : root;
    }

    static Node *FindRightmost(Node *root) {
        return root->right ? FindRightmost(root->right) : root;
    }

    Node *RebalanceFromLeft(Node *root) {
        if (!root->left)
            return root->right;
        root->left = RebalanceFromLeft(root->left);
        return Balance(root);
    }


//    private:

    size_type size_;
    Node *root_;
    Compare comparator_;
//    Alloc alloc_;
//    NodeAlloc node_alloc_;

};

};
#endif //S21_CONTAINERS_S21_SET_H_
