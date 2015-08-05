#pragma once

#include "meta_list.hpp"

template <size_t N>
struct ARLine {
    static constexpr size_t line = N;
};

#define ASSERT_REACHED \
    do { \
        AR::reached[LX::value<>::size()] = true; \
        LX::push<ARLine<__LINE__>>(); \
    } while(0)

#define ASSERT_REACHED_BEGIN(name) \
namespace name { \
struct AR { \
    static bool reached[]; \
    ~AR(); \
}; \
using LX = atch::meta_list<class Example>;

#define ASSERT_REACHED_END \
bool AR::reached[LX::value<>::size()]; \
template <size_t N> \
typename std::enable_if<LX::value<>::size() <= N>::type \
checkValues() { \
} \
template <size_t N> \
typename std::enable_if<N < LX::value<>::size()>::type \
checkValues() { \
    size_t line = LX::value<>::at<N>::result::line; \
    if (AR::reached[N]) { \
        std::cerr << "Reached: " << __FILE__ << ":" << line << "\n"; \
    } else { \
        std::cerr << "Failed to reach: " << __FILE__ << ":" << line << "\n"; \
    } \
    checkValues<N+1>(); \
} \
AR::~AR() { \
    checkValues<0>(); \
} \
AR magic{}; \
} \
