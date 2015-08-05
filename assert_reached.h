#pragma once

#include "meta_list.hpp"
#include "meta_counter.hpp"

using GLX = atch::meta_list<class Counter>;
struct GLXHelper {
    void extra() {
        GLX::push<void>();
    }
};

#define ARG GLX::value<>::at<GLX::value<>::size() - 1>::result

#define ASSERT_REACHED(str) \
    do { \
        struct ARLine {  \
            static void check() { \
                static_assert(! std::is_same<void, ARG>::value, "ASSERT_REACHED not within ASSERT_REACHED_BEGIN/END"); \
                if (ARG::reached[ARG::LX::value<>::size()]) { \
                    std::cerr << "Reached: " << str << " - " << __FILE__ << ":" << __LINE__ << "\n"; \
                } else { \
                    std::cerr << "Failed to reach: " << str << " - " << __FILE__ << ":" << __LINE__ << "\n"; \
                } \
            } \
        }; \
        ARG::reached[ARG::LX::value<>::size()] = true; \
        ARG::LX::push<ARLine>(); \
    } while(0)

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#define ASSERT_REACHED_BEGIN \
namespace { \
struct TOKENPASTE2(AssertReached_, __LINE__) { \
    using LX = atch::meta_list<TOKENPASTE2(AssertReached_, __LINE__)>; \
    ~TOKENPASTE2(AssertReached_, __LINE__)() { cleanup(); } \
    static bool reached[]; \
    struct checkValues; \
    void cleanup(); \
    void extra(); \
}; \
void TOKENPASTE2(AssertReached_, __LINE__)::extra() { \
    GLX::push<TOKENPASTE2(AssertReached_, __LINE__)>(); \
} \
}

#define ASSERT_REACHED_END \
bool ARG::reached[ARG::LX::value<>::size()]; \
struct ARG::checkValues { \
template <size_t N> \
static typename std::enable_if<ARG::LX::value<>::size() <= N>::type \
check() {} \
template <size_t N> \
static typename std::enable_if<N < ARG::LX::value<>::size()>::type \
check() { \
    ARG::LX::value<>::at<N>::result::check(); \
    check<N+1>(); \
} \
}; \
void ARG::cleanup() { \
    checkValues::check<0>(); \
    GLX::pop(); \
}

#define ASSERT_REACHED_GUARD(name) \
    static_assert(! std::is_same<void, ARG>::value, "ASSERT_REACHED_GUARD not within ASSERT_REACHED_BEGIN/END"); \
    ARG name{}
