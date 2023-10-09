#ifndef S21_CONTAINERS_SMC_TESTS_NOMOVE_DUMMY_TEST_H_
#define S21_CONTAINERS_SMC_TESTS_NOMOVE_DUMMY_TEST_H_

namespace s21{
    class NoMoveDummyT{
    public:
        NoMoveDummyT() = default;
        NoMoveDummyT(int y) : x(y){}
        NoMoveDummyT(NoMoveDummyT&&) = delete;
        NoMoveDummyT(const NoMoveDummyT& other){x = other.x;};
        NoMoveDummyT& operator=(const NoMoveDummyT& other){x = other.x; return * this;};
        NoMoveDummyT& operator=(NoMoveDummyT&&) = delete;
        const bool operator<(const NoMoveDummyT& rhs) const{
            return x < rhs.x;
        }
        int x;
    };
    template<typename T>
    class NoMoveDummyComp{
    public:
        NoMoveDummyComp() = default;
        NoMoveDummyComp(int y) : x(y){}
        NoMoveDummyComp(NoMoveDummyComp&&) = delete;
        NoMoveDummyComp& operator=(NoMoveDummyComp&&) = delete;
        NoMoveDummyComp(const NoMoveDummyComp& other){x = other.x;};
        NoMoveDummyComp& operator=(const NoMoveDummyComp& other){x = other.x; return * this;};
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }
        int x;
    };
}

#endif //S21_CONTAINERS_SMC_TESTS_NOMOVE_DUMMY_TEST_H_