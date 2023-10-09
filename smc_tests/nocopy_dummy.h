#ifndef S21_CONTAINERS_SMC_TESTS_NOCOPY_DUMMY_TEST_H_
#define S21_CONTAINERS_SMC_TESTS_NOCOPY_DUMMY_TEST_H_

namespace s21{
    class NoCopyDummyT{
    public:
        NoCopyDummyT() = default;
        NoCopyDummyT(int y) : x(y){}
        NoCopyDummyT(NoCopyDummyT&& other) noexcept {x = other.x;}
        NoCopyDummyT& operator=(NoCopyDummyT&& other) noexcept{x = other.x; return *this;}
        NoCopyDummyT(NoCopyDummyT&) = delete;
        NoCopyDummyT& operator=(NoCopyDummyT&) = delete;
        const bool operator<(const NoCopyDummyT& rhs) const{
            return x < rhs.x;
        }
        int x;
    };
    template<typename T>
    class NoCopyDummyComp{
    public:
        NoCopyDummyComp() = default;
        NoCopyDummyComp(int y) : x(y){}
        NoCopyDummyComp(NoCopyDummyComp&) = delete;
        NoCopyDummyComp& operator=(NoCopyDummyComp&) = delete;
        NoCopyDummyComp(NoCopyDummyComp&& other)noexcept{x = other.x;}
        NoCopyDummyComp& operator=(NoCopyDummyComp&& other)noexcept{x = other.x; return *this;}
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }
        int x;
    };
}

#endif //S21_CONTAINERS_SMC_TESTS_NOCOPY_DUMMY_TEST_H_