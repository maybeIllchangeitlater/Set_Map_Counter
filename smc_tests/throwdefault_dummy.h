#ifndef S21_CONTAINERS_SMC_TESTS_NODEFAULT_DUMMY_TEST_H_
#define S21_CONTAINERS_SMC_TESTS_NODEFAULT_DUMMY_TEST_H_
#include <exception>

namespace s21{
    class NoDefaultDummyT{
    public:
        NoDefaultDummyT() { throw std::exception(); };
        NoDefaultDummyT(int y) : x(y){}
    private:
        int x;
    };
    template<typename T>
    class NoDefaultDummyComp{
    public:
        NoDefaultDummyComp() { throw std::exception(); };
        NoDefaultDummyComp(int y) : x(y){}
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }

    private:
        int x;
    };
}

#endif //S21_CONTAINERS_SMC_TESTS_NODEFAULT_DUMMY_TEST_H_