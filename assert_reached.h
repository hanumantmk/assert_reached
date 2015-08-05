#pragma once

#include "meta_list.hpp"
#include "meta_counter.hpp"

template <size_t N>
struct ARLine {
    static constexpr size_t line = N;
};

#define ASSERT_REACHED \
    do { \
        AR::reached[C1::next()] = true; \
        LX::push<ARLine<__LINE__>>(); \
    } while(0)

#define ASSERT_REACHED_BEGIN \
namespace { \
struct AR { \
    static bool reached[]; \
    ~AR(); \
}; \
using LX = atch::meta_list<class Example>; \
using C1 = atch::meta_counter<class Example>; \

#define ASSERT_REACHED_END \
bool AR::reached[C1::value() + 1]; \
template <size_t N> \
typename std::enable_if<C1::value() < N>::type \
checkValues() { \
} \
template <size_t N> \
typename std::enable_if<N <= C1::value()>::type \
checkValues() { \
    size_t line = LX::value<>::at<N - 1>::result::line; \
    if (AR::reached[N]) { \
        std::cerr << "Reached: " << __FILE__ << ":" << line << "\n"; \
    } else { \
        std::cerr << "Failed to reach: " << __FILE__ << ":" << line << "\n"; \
    } \
    checkValues<N+1>(); \
} \
AR::~AR() { \
    checkValues<1>(); \
} \
AR magic{}; \
} \
