#ifndef S21_CONTAINERS_SMC_TESTS_NODEFAULT_DUMMY_TEST_H_
#define S21_CONTAINERS_SMC_TESTS_NODEFAULT_DUMMY_TEST_H_
#include <type_traits>
#include <iostream>
namespace s21{
    class NoDefaultDummyT{
    public:
        NoDefaultDummyT() = delete;
        NoDefaultDummyT(int y) : x(y){}
        NoDefaultDummyT(const NoDefaultDummyT& other) : x(other.x){}
        NoDefaultDummyT& operator=(const NoDefaultDummyT& other) {x = other.x; return *this;}
        NoDefaultDummyT(NoDefaultDummyT&& other) noexcept : x(std::move(other.x)) {}
        NoDefaultDummyT& operator=(NoDefaultDummyT&& other) noexcept{ x= other.x; return *this;}
        bool operator<(const NoDefaultDummyT& rhs) const{
            return x < rhs.x;
        }
        int x;
    };
    template<typename T>
    class NoDefaultDummyComp{
    public:
        NoDefaultDummyComp() = delete;
        NoDefaultDummyComp(int y) : x(y){}
        NoDefaultDummyComp(const NoDefaultDummyComp& other) : x(other.x){}
        NoDefaultDummyComp(NoDefaultDummyComp&& other) noexcept : x(std::move(other.x)) {}

        template<typename U = NoDefaultDummyComp<T>>
        void check(){
            if(std::is_copy_constructible_v<U>)
                std::cout << "COPY ME";
        }
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }
        int x;
    };
}

#endif //S21_CONTAINERS_SMC_TESTS_NODEFAULT_DUMMY_TEST_H_