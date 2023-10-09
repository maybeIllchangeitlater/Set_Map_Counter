#ifndef S21_CONTAINERS_SMC_TESTS_NOCOPY_DUMMY_TEST_H_
#define S21_CONTAINERS_SMC_TESTS_NOCOPY_DUMMY_TEST_H_
#include <exception>
namespace s21{
    class NoCopyDummyT{
    public:
        NoCopyDummyT();
        NoCopyDummyT(int y) : x(y){}
        NoCopyDummyT(NoCopyDummyT&) {throw std::exception();};
        NoCopyDummyT& operator=(NoCopyDummyT&) {throw std::exception();};
        int x;
    };
    template<typename T>
    class NoCopyDummyComp{
    public:
        NoCopyDummyComp();
        NoCopyDummyComp(int y) : x(y){}
        NoCopyDummyComp(NoCopyDummyComp&) {throw std::exception();};
        NoCopyDummyComp& operator=(NoCopyDummyComp&) {throw std::exception();};
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }
        int x;
    };
}

#endif //S21_CONTAINERS_SMC_TESTS_NOCOPY_DUMMY_TEST_H_