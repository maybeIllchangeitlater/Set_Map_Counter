#ifndef S21_CONTAINERS_SMC_TESTS_NOMOVE_DUMMY_TEST_H_
#define S21_CONTAINERS_SMC_TESTS_NOMOVE_DUMMY_TEST_H_
#include <exception>
namespace s21{
    class NoMoveDummyT{
    public:
        NoMoveDummyT();
        NoMoveDummyT(int y) : x(y){}
        NoMoveDummyT(NoMoveDummyT&&) {throw std::exception();};
        NoMoveDummyT& operator=(NoMoveDummyT&&) {throw std::exception();};
    private:
        int x;
    };
    template<typename T>
    class NoMoveDummyComp{
    public:
        NoMoveDummyComp();
        NoMoveDummyComp(int y) : x(y){}
        NoMoveDummyComp(NoMoveDummyComp&&) {throw std::exception();};
        NoMoveDummyComp& operator=(NoMoveDummyComp&&){throw std::exception();};
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }

    private:
        int x;
    };
}

#endif //S21_CONTAINERS_SMC_TESTS_NOMOVE_DUMMY_TEST_H_