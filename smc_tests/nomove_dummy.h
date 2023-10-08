#ifndef S21_CONTAINERS_SMC_TESTS_NOMOVE_DUMMY_TEST_H_
#define S21_CONTAINERS_SMC_TESTS_NOMOVE_DUMMY_TEST_H_

namespace s21{
    class NoMoveDummyT{
    public:
        NoMoveDummyT();
        NoMoveDummyT(int y) : x(y){}
        NoMoveDummyT(NoMoveDummyT&&) = delete;
        NoMoveDummyT& operator=(NoMoveDummyT&&) = delete;
    private:
        int x;
    };
    template<typename T>
    class NoMoveDummyComp{
    public:
        NoMoveDummyComp();
        NoMoveDummyComp(int y) : x(y){}
        NoMoveDummyComp(NoMoveDummyComp&&) = delete;
        NoMoveDummyComp& operator=(NoMoveDummyComp&&) = delete;
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }

    private:
        int x;
    };
}

#endif //S21_CONTAINERS_SMC_TESTS_NOMOVE_DUMMY_TEST_H_