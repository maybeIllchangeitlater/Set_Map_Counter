#ifndef S21_CONTAINERS_SMC_TESTS_NOCOPY_DUMMY_TEST_H_
#define S21_CONTAINERS_SMC_TESTS_NOCOPY_DUMMY_TEST_H_

namespace s21{
    class NoCopyDummyT{
    public:
        NoCopyDummyT();
        NoCopyDummyT(int y) : x(y){}
        NoCopyDummyT(NoCopyDummyT&) = delete;
        NoCopyDummyT& operator=(NoCopyDummyT&) = delete;
    private:
        int x;
    };
    template<typename T>
    class NoCopyDummyComp{
    public:
        NoCopyDummyComp();
        NoCopyDummyComp(int y) : x(y){}
        NoCopyDummyComp(NoCopyDummyComp&) = delete;
        NoCopyDummyComp& operator=(NoCopyDummyComp&) = delete;
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }

    private:
        int x;
    };
}

#endif //S21_CONTAINERS_SMC_TESTS_NOCOPY_DUMMY_TEST_H_