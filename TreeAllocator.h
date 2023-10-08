#ifndef S21_CONTAINERS_TREEALLOCATOR_H_
#define S21_CONTAINERS_TREEALLOCATOR_H_

#include <memory>
#include <vector>
#include <iostream>
#include <ctype.h>

/**
 * @brief usable with LLVM (clang++) compiler RBTrees but not with GNU (GCC) compilers\n
 * should work for any linked list like structure given same pointer name (__left_)
 */
namespace s21 {
    template<typename T>
    class MyTreeAllocator final {
    public:
        using value_type = T;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using is_always_equal = std::false_type;
        using void_pointer = void*;
        using const_void_pointer = const void_pointer;


        MyTreeAllocator() noexcept {
            ++ref_count_;
//            std::cout << "new allocator, refcount is: " << ref_count_ << std::endl;
        }

        MyTreeAllocator(const MyTreeAllocator&) noexcept : MyTreeAllocator() {}

        MyTreeAllocator& operator=(const MyTreeAllocator&) noexcept {
            return *this;
        };

        MyTreeAllocator(MyTreeAllocator&&) noexcept : MyTreeAllocator() {
        };

        MyTreeAllocator& operator=(MyTreeAllocator&&) noexcept {
            return *this;
        }

        /**
         * @brief logic for rebinded allocator construct. Idk for now
         * Technically I need refcount only for rebound(node) allocs
         */
        template<typename U>
        MyTreeAllocator(const MyTreeAllocator<U>&) noexcept : MyTreeAllocator() {
        }

        template <typename U>
        MyTreeAllocator& operator=(const MyTreeAllocator<U>&) noexcept {
            return *this;
        }
        template<typename U>
        MyTreeAllocator(MyTreeAllocator<U>&&) noexcept : MyTreeAllocator(){
        };
        template<typename U>
        MyTreeAllocator& operator=(MyTreeAllocator<U>&&) noexcept{
        };

        /**
         * @brief all memory allocated during trees lifecycles gets yeeted here
         */
        ~MyTreeAllocator() {
            --ref_count_;
//            std::cout << "allocator dies, refcount is: " << ref_count_ << std::endl;
            if (!ref_count_) {
                int n = for_deletion_.size();
                for (int i = 0; i < n; ++i) {
//                    std::cout << "ABOBA DOESNT LEAK" << std::endl;
                    ::operator delete[](for_deletion_[i]);
                }
            }
        }

        /**
         * @brief rebind T to node T
         */
        template<typename U>
        struct rebind {
            using other = MyTreeAllocator<U>;
        };

        /**
         * @brief gets called only for Node
         * reallocs 1.5 if no allocate memory left otherwise takes empty(left) node, puts it to the right
         * and returns it
         * make sure n is 1
         * prehaps smarter move to make use of n?
         */

        [[nodiscard]] pointer allocate(const size_type n) {
            if(!n){
                throw std::bad_alloc();
            }
            if (!reusable_ || !reusable_->__left_) {
                for_deletion_.push_back(
                        static_cast<pointer>(::operator new[](n * sizeof(value_type) * allocate_this_)));
                for_deletion_.back()[0].__left_ = nullptr;
                for (size_type i = 1; i < allocate_this_ * n; ++i)
                    for_deletion_.back()[i].__left_ = &(for_deletion_.back()[i - 1]);

                reusable_ ? reusable_->__left_ = &(for_deletion_.back()[allocate_this_ - 1]) :
                        reusable_ = &(for_deletion_.back()[allocate_this_ - 1]);

                allocate_this_ *= alloc_factor_ * n;
            }

            auto ret = reusable_->__left_;
            reusable_->__left_ = reusable_->__left_->__left_;

            return static_cast<pointer>(ret);
        }

        /**
         * @brief doesn't dealloc anything. Only called for Node T
         * moves node to the left from reusable so it can be reused again
         * implies call to destroy for T beforehands
         */
        void deallocate(const pointer ptr, const size_type n) {
            if(n){}
            ptr->__left_ = reusable_->__left_;
            reusable_->__left_ = ptr;
        }

        /**
         * @brief placement new. Only gets called for T. No logic here, can be replaced
         * with default implementation from allocator_traits
         */
        template<typename U, typename... Args>
        void construct(U *ptr, Args &&... args) {
            new(ptr) U(std::forward<Args>(args)...);
        }

        /**
         * @brief can be replaced with default allocator_traits implementation
         * Only gets called for T
         */
        void destroy(const pointer ptr) {
            ptr->~T();
        }


        bool operator==(const MyTreeAllocator &) const { return true; }
        bool operator!=(const MyTreeAllocator &) const { return false; }
        template <typename U>
        bool operator==(const MyTreeAllocator<U>&) const noexcept { return true; }
        template <typename U>
        bool operator!=(const MyTreeAllocator<U>&) const noexcept { return false; }

        pointer address(reference r) { return &r; } //very questionable
        const_pointer address(const_reference s) { return &s; }

    private:
        constexpr static const float alloc_factor_ = 1.5;
        static size_type allocate_this_;
        static pointer reusable_;
        static std::vector<pointer> for_deletion_;
        static size_type ref_count_;

    };

////// inits refcount and vec for allocator so it can be reused and is not destroyed by 1 set if 3 sets are using it
    template<typename T>
    typename MyTreeAllocator<T>::size_type MyTreeAllocator<T>::allocate_this_ = 10;
    template<typename T>
    typename MyTreeAllocator<T>::pointer MyTreeAllocator<T>::reusable_ = nullptr;
    template<typename T>
    size_t MyTreeAllocator<T>::ref_count_ = 0;
    template<typename T>
    std::vector<typename MyTreeAllocator<T>::pointer> MyTreeAllocator<T>::for_deletion_;
} //namespace s21

#endif //S21_CONTAINERS_TREEALLOCATOR_H_
