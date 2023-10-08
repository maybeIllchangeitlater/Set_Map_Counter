#ifndef S21_CONTAINERS_SMC_TESTS_NODEFAULT_DUMMY_TEST_H_
#define S21_CONTAINERS_SMC_TESTS_NODEFAULT_DUMMY_TEST_H_

namespace s21{
    class NoDefaultDummyT{
    public:
        NoDefaultDummyT() = delete;
        NoDefaultDummyT(int y) : x(y){}
        const bool operator<(const NoDefaultDummyT& rhs) const{
            return x < rhs.x;
        }
    private:
        int x;
    };
    template<typename T>
    class NoDefaultDummyComp{
    public:
        NoDefaultDummyComp() = delete;
        NoDefaultDummyComp(int y) : x(y){}
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }

    private:
        int x;
    };
}

#endif //S21_CONTAINERS_SMC_TESTS_NODEFAULT_DUMMY_TEST_H_